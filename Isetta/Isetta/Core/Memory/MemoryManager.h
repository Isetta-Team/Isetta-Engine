#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {

class MemoryAllocator {
 private:
  static void* AllocateDefaultAligned(SizeInt size);
  static void FreeDefaultAligned(void*);

  /**
   * \brief
   * \tparam
   * \param size
   * \param alignment Alignment has to be power of 2
   * \return a raw pointer to the newly allocated memory address
   */
  static void* AllocateAligned(SizeInt size, U8 alignment);
  static void FreeAligned(void*);

  static class StackAllocator singleFrameAllocator;

  friend class StackAllocator;
  template <typename T>
  friend class PoolAllocator;
};

}  // namespace Isetta
