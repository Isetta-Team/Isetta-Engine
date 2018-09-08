#pragma once

#include "IsettaTypes.h"

namespace Isetta {

void* AllocateUnaligned(U32 size);
void FreeUnaligned(void*);

/**
 * \brief 
 * \tparam 
 * \param size 
 * \param alignment Alignment has to be power of 2
 * \return a raw pointer to the newly allocated memory address
 */
void* AllocateAligned(U32 size, U8 alignment);
void FreeAligned(void*);

class StackAllocator {
 public:
  typedef U32 Marker;

  explicit StackAllocator(U32 stackSize);

  /**
   * \brief You probably want to manually call constructor after getting this memory
   * otherwise the new object may not function properly
   * \param sizeInBytes Number of bytes you want
   * \return pointer to the allocated memory
   */
  //TODO: alignment of the memory
  void* Alloc(U32 sizeInBytes);

  template <typename T>
  T* New();

  template <typename T>
  T* New(Marker& marker);

  /**
   * \brief
   * \param marker
   */
  void FreeToMarker(const Marker marker) { top = marker; };
  void Clear() { top = 0; };
  void Erase();
  Marker GetMarker() const { return top; };

 private:
  Marker top;
  U32 capacity;
  char* bottom;
};

template <typename T>
T* StackAllocator::New() {
  void* mem = Alloc(sizeof(T));
  return new (mem) T();
}

template <typename T>
T* StackAllocator::New(Marker& marker) {
  void* mem = Alloc(sizeof(T));
  marker = top;
  return new (mem) T();
}

class PoolAllocator {};

}  // namespace Isetta