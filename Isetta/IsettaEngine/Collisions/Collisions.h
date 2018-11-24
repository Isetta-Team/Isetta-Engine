/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "ISETTA_API.h"

namespace Isetta {
class ISETTA_API_DECLARE Collisions {
 public:
  static bool Raycast(const class Ray &ray, class RaycastHit *const hitInfo,
                      float maxDistance = 0);
  // TODO(Jacob) ColliderCasts? LineCast
  // TODO(Jacob) CheckCollider - check for overlap
  // TODO(Jacob) OverlapCollider - touching or inside
  // TOOD(Jacob) ClosestPoint
  // TODO(Jacob) ComputePenetration (not really needed)
  bool GetIgnoreLayerCollision(int layer1, int layer2) const;
  void SetIgnoreLayerCollision(int layer1, int layer2, bool ignoreLayer = true);
  static void IgnoreCollisions(class Collider *const a, class Collider *const b,
                               bool ignore = true);

 private:
  static class CollisionsModule *collisionsModule;
  friend class CollisionsModule;
};
}  // namespace Isetta
