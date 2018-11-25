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
class MemUtil {
 public:
  static const U8 ALIGNMENT = 16;
  static void CheckAlignment(U8 alignment);

  /**
   * \brief Allocate properly aligned memory. The adjusted size of memory is
   * stored as a head (1 byte to the left of the returned raw memory pointer)
   *
   * \param size Size in bytes
   * \param alignment Alignment requirement. Must be power of 2 and less than
   * 128 and >= 8
   * \return A raw pointer to the newly allocated memory address
   */
  static void* Alloc(Size size, U8 alignment = ALIGNMENT);

  /**
   * \brief Free the properly aligned memory address
   */
  static void Free(void*);
  template<typename T>
  static std::string GetNameForType() {
    std::string name = typeid(T).name();
    auto pos = name.find("::");
    if (pos != std::string::npos) {
      name = name.substr(pos + 2);
    }

    pos = name.find(" * __ptr64");
    if (pos != std::string::npos) {
      name = name.substr(0, pos);
      name += "*";
    }
    
    return name;
  }
};

}  // namespace Isetta
