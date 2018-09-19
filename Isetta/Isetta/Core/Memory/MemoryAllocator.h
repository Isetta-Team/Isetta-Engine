/*
* Copyright (c) 2018 Isetta
*/
#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {

class MemoryAllocator {
 private:
  static void* AllocateDefaultAligned(Size size);
  static void FreeDefaultAligned(void*);

  /**
   * \brief
   * \tparam
   * \param size
   * \param alignment Alignment has to be power of 2
   * \return a raw pointer to the newly allocated memory address
   */
  static void* AllocateAligned(Size size, U8 alignment);
  static void FreeAligned(void*);

  friend class StackAllocator;
  friend class PoolAllocator;
  template <typename T>
  friend class TemplatePoolAllocator;
};

}  // namespace Isetta
