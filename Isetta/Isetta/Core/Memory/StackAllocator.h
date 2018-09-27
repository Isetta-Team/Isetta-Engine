/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {
class StackAllocator {
 public:
  typedef Size Marker;

  StackAllocator() = default;
  explicit StackAllocator(Size stackSize);
  ~StackAllocator() = default;

  /**
   * \brief Grab properly aligned memory from the stack allocator. You probably
   * want to `GetMarker()` before calling this if you want to free to that
   * marker
   *
   * \param size Number of bytes you want, in byte
   * \param alignment Alignment requirement of the memory, must be power of 2
   * and less than 128 \return pointer to the allocated memory
   * \return Pointer to allocated memory
   */
  void* Alloc(Size size, U8 alignment = 16);

  /**
   * \brief Create a new object on the stack allocator. The constructor is
   * automatically called. The memory is 16 aligned by default. If you are using
   * this, you probably need to call the destructor on your own.
   *
   * \tparam T type of object you want to create
   * \return Pointer to new object
   */
  template <typename T>
  T* New();

  /**
   * \brief Similar to `New<T>()`, but you have to manually pass in a reference
   * to a marker, and that marker's value will be changed in this method
   *
   * \tparam T type of object you want to create
   * \return Pointer to new object
   */
  template <typename T>
  T* New(Marker& outMarker);

  /**
   * \brief Free the stack allocator to a specific marker
   * \param marker Marker to free to
   */
  void FreeToMarker(const Marker marker) { top = marker; };

  /**
   * \brief Clear the whole stack allocator to its bottom. All memory will be
   * available for new allocations again
   */
  void Clear() { top = 0; };

  /**
   * \brief Free all memory in this stack allocator. The stack allocator will
   * become unusable after calling this
   */
  void Erase();

  /**
   * \brief Get the current marker position
   * \return 
   */
  Marker GetMarker() const { return top; };

 private:
  Marker top;
  Size length;
  void* bottom;
  PtrInt bottomAddress;
};

template <typename T>
T* StackAllocator::New() {
  void* mem = Alloc(sizeof(T));
  return new (mem) T();
}

template <typename T>
T* StackAllocator::New(Marker& outMarker) {
  outMarker = top;
  return New<T>();
}
}  // namespace Isetta
