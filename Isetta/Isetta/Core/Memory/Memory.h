#pragma once

#include <cstdlib>
#include "IsettaTypes.h"

namespace Isetta {

// class MemoryManager {
  // public:
   // static void* AllocateUnaligned(U32);
   // static void* AllocateAligned(U32);

   // static void Free(void*);
// };

void* AllocateUnaligned(U32);
void* AllocateAligned(U32 size, U32 alignment);

void Free(void*);

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