/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {
/**
 * \brief A double buffered allocator is a wrapper on top of the stack
 * allocator. It holds two stack allocators (A, B) and marks one of them as an
 * active stack allocator each frame. At the end of each frame, it swaps A and B
 * and clears the newly activated one. For example, if A is the active allocator
 * of frame n, all memory allocation requests will be handled by A during frame
 * n. At the end of frame n, B becomes the active allocator and is cleared. So
 * during frame n + 1, memory allocation will be done by B, and those pointers
 * to memory in A are still valid. At the end of n + 1, A is again activated and
 * cleared.
 */
class DoubleBufferedAllocator {
 private:
  DoubleBufferedAllocator() = delete;
  explicit DoubleBufferedAllocator(Size size);
  ~DoubleBufferedAllocator() = default;

  /**
   * \brief Grab properly aligned raw memory from the double buffered allocator.
   * The memory will be automatically cleared at the end of next frame
   *
   * \param size Size in bytes
   * \param alignment Alignment requirement for this memory. Must be power of 2
   * and less than 256 \return Raw memory pointer, use with caution!
   */
  void* Alloc(Size size, U8 alignment = 16);

  /**
   * \brief Swap the active stack allocator
   */
  void SwapBuffer();

  /**
   * \brief Clears the currently active stack allocator
   */
  void ClearCurrentBuffer();

  /**
   * \brief Creates a new object on the active stack allocator, which will
   * become invalid at the end of next frame. You need to manually call
   * destructor on the object if needed. 
   * 
   * \tparam T Type of object
   * \tparam args Arguments for the constructor
   * \return Pointer to new object
   */
  template <typename T, typename ...args>
  T* New(args...);

  template<typename T>
  T* NewArr(Size length, U8 alignment);

  StackAllocator stacks[2];
  U8 curStackIndex;
  friend class MemoryManager;
};

template <typename T, typename ...args>
T* DoubleBufferedAllocator::New(args... argList) {
  return stacks[curStackIndex].New<T>(argList...);
}

template <typename T>
T* DoubleBufferedAllocator::NewArr(const Size length, const U8 alignment) {
  return stacks[curStackIndex].NewArr<T>(length, alignment);
}

}  // namespace Isetta
