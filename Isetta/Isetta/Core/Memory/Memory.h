#pragma once
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

class PoolAllocator {
public:

private:
  SizeInt capacity;
  SizeInt slotSize;
};

}  // namespace Isetta