/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Physics/Collider.h"

namespace Isetta::Math {
class Matrix4;
}

namespace Isetta {
class CapsuleCollider : public Collider {
 private:
 public:
  enum class Direction { X_AXIS, Y_AXIS, Z_AXIS };

  float radius, height;
  Direction direction;

  void Update() override;

  float TransformUtility(Math::Matrix4& rotation, Math::Matrix4 scale) const;
};
}  // namespace Isetta
