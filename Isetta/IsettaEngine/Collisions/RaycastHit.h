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
  /**
   * @brief Get the collider object of hit
   *
   * @return Collider*
   */
  inline class Collider* GetCollider() { return collider; }
  /**
   * @brief Get the distance from ray origin to collider
   *
   * @return float
   */
  inline float GetDistance() { return distance; }
  /**
   * @brief Get the point of intersection
   *
   * @return Math::Vector3
   */
  inline Math::Vector3 GetPoint() { return point; }
  /**
   * @brief Get the normal of intersection
   * [Warning] Not fully implemented for all collider types
   *
   * @return Math::Vector3
   */
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
