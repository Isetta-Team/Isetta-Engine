/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "ISETTA_API.h"

namespace Isetta {
struct ISETTA_API Primitive {
  enum class Type {
    Capsule,
    Cube,
    Cylinder,
    Grid,
    Quad,
    Sphere,
  };
  /**
   * \brief Create a primitive with the given name, with the option to specific
   * if it has a collider attached
   */
  static class Entity* Create(Type type, std::string name, bool withCollider);
  /**
   * \brief Create a primitive, with the option to specific if it has a collider
   * attached
   */
  static class Entity* Create(Type type, bool withCollider = false);
};
}  // namespace Isetta