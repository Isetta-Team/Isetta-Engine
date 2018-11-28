/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "ISETTA_API.h"
#include <string>

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
  static class Entity* Create(Type type, std::string name, bool withCollider);
  static class Entity* Create(Type type, bool withCollider = false);
};
}  // namespace Isetta