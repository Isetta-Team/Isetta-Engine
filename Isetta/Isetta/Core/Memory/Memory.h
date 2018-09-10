#pragma once
#include <stdexcept>
#include "IsettaTypes.h"

namespace Isetta {

class MemoryManager {
 private:
  static void* AllocateUnaligned(SizeInt size);
  static void FreeUnaligned(void*);

  /**
   * \brief
   * \tparam
   * \param size
   * \param alignment Alignment has to be power of 2
   * \return a raw pointer to the newly allocated memory address
   */
  static void* AllocateAligned(SizeInt size, U8 alignment);
  static void FreeAligned(void*);

  friend class StackAllocator;
  template <typename T>
  friend class PoolAllocator;
};

class StackAllocator {
 public:
  typedef SizeInt Marker;

  explicit StackAllocator(SizeInt stackSize);

  /**
   * \brief You probably want to manually call constructor after getting this
   * memory otherwise the new object may not function properly \param
   * sizeInBytes Number of bytes you want \return pointer to the allocated
   * memory
   */
  void* AllocAligned(SizeInt size, U8 alignment = 16);
  void* AllocUnaligned(SizeInt size);

  /**
   * \brief If you are using this, you probably need to call the destructor on
   * your own
   * \tparam T
   * \return
   */
  template <typename T>
  T* New() {
    void* mem = AllocAligned(sizeof(T));
    return new (mem) T();
  };

  template <typename T>
  T* New(Marker& outMarker) {
    outMarker = top;
    return New<T>();
  };

  void FreeToMarker(const Marker marker) { top = marker; };
  void Clear() { top = 0; };
  void Erase();
  Marker GetMarker() const { return top; };

 private:
  Marker top;
  SizeInt length;
  void* bottom;
  PtrInt bottomAddress;
};

template <typename T>
class PoolAllocator {
 public:
  explicit PoolAllocator(SizeInt count);
  ~PoolAllocator();
  T* Get();
  void Free(T*);

 private:
  union Node {
    T t;
    Node* next;
    Node();
  };

  SizeInt capacity;
  SizeInt elementSize;
  Node* head;
  void* memHead;
  bool isErased;
};

template <typename T>
PoolAllocator<T>::PoolAllocator(const SizeInt count) {
  isErased = false;
  capacity = count;
  elementSize = sizeof(T);
  memHead = MemoryManager::AllocateUnaligned(elementSize * capacity);
  PtrInt address = reinterpret_cast<PtrInt>(memHead);
  head = new (memHead) Node();
  Node* cur = head;

  for (int i = 0; i < count; i++) {
    address += elementSize;
    Node* node = new (reinterpret_cast<void*>(address)) Node();
    cur->next = node;
    cur = cur->next;
  }
}

template <typename T>
PoolAllocator<T>::~PoolAllocator() {
  isErased = true;
  MemoryManager::FreeUnaligned(memHead);
}

template <typename T>
T* PoolAllocator<T>::Get() {
  if (isErased) {
    throw std::exception("PoolAllocator::Get(): PoolAllocator already erased");
  }

  if (head == nullptr) {
    throw std::out_of_range(
        "PoolAllocator::Get(): Not enough memory in PoolAllocator");
  }

  Node* next = head->next;
  T* t = new (head) T();
  head = next;
  return t;
}

template <typename T>
void PoolAllocator<T>::Free(T* t) {
  if (isErased) {
    throw std::exception("PoolAllocator::Free(): PoolAllocator already erased");
  }
  t->~T();
  Node* node = new (t) Node();
  node->next = head;
  head = node;
}

template <typename T>
PoolAllocator<T>::Node::Node() {
  next = nullptr;
}

}  // namespace Isetta