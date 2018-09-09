#pragma once

#include "IsettaTypes.h"

namespace Isetta {

class MemoryManager {
 public:
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

  // TODO: find a proper way to delete placement new
  template <typename T>
  T* New();

  template <typename T>
  T* New(Marker& marker);

  void FreeToMarker(const Marker marker) { top = marker; };
  void Clear() { top = 0; };
  void Erase();
  Marker GetMarker() const { return top; };

 private:
  Marker top;
  SizeInt capacity;
  void* bottom;
  PtrInt bottomAddress;
};

template <typename T>
T* StackAllocator::New() {
  void* mem = AllocAligned(sizeof(T));
  return new (mem) T();
}

template <typename T>
T* StackAllocator::New(Marker& marker) {
  void* mem = AllocAligned(sizeof(T));
  marker = top;
  return new (mem) T();
}

class PoolAllocator {};

}  // namespace Isetta