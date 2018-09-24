/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {

/**
 * \brief MemoryAllocator class provides some handy utilities for allocating and
 * freeing memory. This is only intended to be used by the programmer
 * responsible for maintaining memory management systems of this engine.
 */
class MemoryAllocator {
 private:
  /**
   * \brief Allocate default-aligned memory by calling std::malloc. This always
   * return 16 aligned memory on the author's computer but it might be different
   * on yours so use with caution.
   * 
   * \param size Size in bytes
   * \return Raw memory pointer
   */
  static void* AllocateDefaultAligned(Size size);

  /**
   * \brief Free a default-aligned memory chunk
   */
  static void FreeDefaultAligned(void*);

  /**
   * \brief Allocate properly aligned memory. The adjusted size of memory is
   * stored as a head (1 byte to the left of the returned raw memory pointer)
   * 
   * \param size Size in bytes
   * \param alignment Alignment requirement. Must be power of 2 and less than
   * 128
   * \return A raw pointer to the newly allocated memory address
   */
  static void* AllocateAligned(Size size, U8 alignment);

  /**
   * \brief Free the properly aligned memory address
   */
  static void FreeAligned(void*);

  friend class StackAllocator;
  friend class PoolAllocator;
  template <typename T>
  friend class TemplatePoolAllocator;
};

}  // namespace Isetta
