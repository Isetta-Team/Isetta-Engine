/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/DataStructures/Array.h"
#include "ISETTA_API.h"

namespace Isetta {
class ISETTA_API_DECLARE Collisions {
 public:
  /**
   * @brief Raycast all colliders to detect for intersection/hit, return hit
   * with closest
   *
   * @param ray which is being cast
   * @param hitInfo the closest successful intersection
   * @param maxDistance the ray can travel
   * @return true if an intersection occured
   */
  static bool Raycast(const class Ray &ray, class RaycastHit *const hitInfo,
                      float maxDistance = 0);
  /**
   * @brief Raycast all colliders to detect for intersection/hit, return all
   * colliders hit
   *
   * @param ray which is being cast
   * @param maxDistance the ray can travel
   * @return Array<RaycastHit> array of successful intersections
   */
  static Array<RaycastHit> RaycastAll(const class Ray &ray,
                                      float maxDistance = 0);
  // TODO(Jacob) ColliderCasts? LineCast
  // TODO(Jacob) CheckCollider - check for overlap
  // TODO(Jacob) OverlapCollider - touching or inside
  // TOOD(Jacob) ClosestPoint
  // TODO(Jacob) ComputePenetration (not really needed)

  /**
   * @brief Get if layers ignore collisions
   *
   * @return true if collisions are ignored between the 2 layers
   */
  static bool GetIgnoreLayerCollision(int, int);
  /**
   * @brief Set to ignore collisions between layers
   *
   * @param ignoreLayer true if the two layers ignore collisions
   */
  static void SetIgnoreLayerCollision(int, int, bool ignoreLayer = true);
  /**
   * @brief Set to ignore collisions between 2 colliders
   *
   * @param ignore true if the 2 colliders ignore collisions
   */
  static void SetIgnoreCollisions(class Collider *const, class Collider *const,
                                  bool ignore = true);
  /**
   * @brief Get if 2 colliders ignore collisions
   *
   */
  static bool GetIgnoreCollisions(class Collider *const, class Collider *const);

 private:
  static class CollisionsModule *collisionsModule;
  friend class CollisionsModule;
};
}  // namespace Isetta
