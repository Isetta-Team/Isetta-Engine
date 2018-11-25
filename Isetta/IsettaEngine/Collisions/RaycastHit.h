/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/Math/Vector3.h"

namespace Isetta {
class ISETTA_API_DECLARE RaycastHit {
 private:
  class Collider* collider;
  float distance{INFINITY};
  Math::Vector3 point, normal;

  RaycastHit(class Collider* collider, float distance, Math::Vector3 point,
             Math::Vector3 normal)
      : collider{collider}, distance{distance}, point{point}, normal{normal} {}

 public:
  inline class Collider* GetCollider() { return collider; }
  inline float GetDistance() { return distance; }
  inline Math::Vector3 GetPoint() { return point; }
  inline Math::Vector3 GetNormal() { return normal; }

  RaycastHit() = default;
  RaycastHit& operator=(RaycastHit&& inHit) noexcept {
    collider = inHit.collider;
    inHit.collider = nullptr;
    distance = inHit.distance;
    inHit.distance = INFINITY;
    point = inHit.point;
    normal = inHit.normal;
    return *this;
  }

  friend class Collider;
  friend class Plane;
  friend class AABB;
  friend class BVTree;
};
}  // namespace Isetta
