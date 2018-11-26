/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "ISETTA_API.h"
#include "Core/DataStructures/Array.h"

namespace Isetta {
class ISETTA_API_DECLARE Collisions {
 public:
  static bool Raycast(const class Ray &ray, class RaycastHit *const hitInfo,
                      float maxDistance = 0);
  static Array<RaycastHit> RaycastAll(const class Ray &ray, float maxDistance = 0);
  // TODO(Jacob) ColliderCasts? LineCast
  // TODO(Jacob) CheckCollider - check for overlap
  // TODO(Jacob) OverlapCollider - touching or inside
  // TOOD(Jacob) ClosestPoint
  // TODO(Jacob) ComputePenetration (not really needed)
  static bool GetIgnoreLayerCollision(int layer1, int layer2);
  static void SetIgnoreLayerCollision(int layer1, int layer2,
                                      bool ignoreLayer = true);
  static void IgnoreCollisions(class Collider *const a, class Collider *const b,
                               bool ignore = true);

 private:
  static class CollisionsModule *collisionsModule;
  friend class CollisionsModule;
};
}  // namespace Isetta
