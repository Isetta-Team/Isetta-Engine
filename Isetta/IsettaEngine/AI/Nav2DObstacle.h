/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/DataStructures/Array.h"
#include "ISETTA_API.h"

namespace Isetta::Math {
class Vector2;
class Rect;
}  // namespace Isetta::Math

namespace Isetta {
struct ISETTA_API Nav2DObstacle {
  Array<Math::Vector2> points;
  Nav2DObstacle() = default;
  explicit Nav2DObstacle(const Array<Math::Vector2>& points);

  static Nav2DObstacle Rectangle(const Math::Rect& rectangle);
  static Nav2DObstacle Circle(const Math::Vector2& position, float radius,
                              float angleOffset = 0, int segments = 12);

#ifdef _EDITOR
  void DebugDraw() const;
#endif

  // TODO(all): Add obstacles as cross-sections of colliders (either maximum
  // area or cross section intersecting nav plane)
  // Nav2DObstacle(const class BoxCollider& collider, bool useYPos = false);
  // Nav2DObstacle(const class SphereCollider& collider, int segments = 12,
  //              bool useYPos = false);
  // Nav2DObstacle(const class CapsuleCollider& collider, int segments = 12,
  //              bool useYPos = false);
};
}  // namespace Isetta
