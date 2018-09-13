/*
* Copyright (c) 2018 Isetta
*/
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemoryManager.h"

namespace Isetta {

template <typename T>
class PoolAllocator {
 public:
  explicit PoolAllocator(SizeInt count);
  ~PoolAllocator();
  T* Get();
  void Free(T*);
  void Erase();

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
  memHead = MemoryAllocator::AllocateDefaultAligned(elementSize * capacity);
  PtrInt address = reinterpret_cast<PtrInt>(memHead);
  head = new (memHead) Node();
  Node* cur = head;

  for (SizeInt i = 1; i < count; i++) {
    address += elementSize;
    Node* node = new (reinterpret_cast<void*>(address)) Node();
    cur->next = node;
    cur = cur->next;
  }
}

template <typename T>
PoolAllocator<T>::~PoolAllocator() {
  if (isErased) {
    return;
  }
  isErased = true;
  MemoryAllocator::FreeDefaultAligned(memHead);
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
void PoolAllocator<T>::Erase() {
  if (isErased) {
    throw std::exception("PoolAllocator::Get(): PoolAllocator already erased");
  }
  isErased = true;
  MemoryAllocator::FreeDefaultAligned(memHead);
}

template <typename T>
PoolAllocator<T>::Node::Node() {
  next = nullptr;
}
}  // namespace Isetta