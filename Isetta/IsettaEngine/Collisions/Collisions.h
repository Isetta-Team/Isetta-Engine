/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
class Collisions {
 public:
  static bool Raycast(const class Ray &ray, class RaycastHit *const hitInfo,
                      float maxDistance = 0);
  // TODO(Jacob) ColliderCasts? LineCast
  // TODO(Jacob) CheckCollider - check for overlap
  // TOOD(Jacob) ClosestPoint
  // TODO(Jacob) ComputePenetration (not really needed)
  // TODO(Jacob) GetIgnoreLayerCollision
  // TODO(Jacob) IgnoreCollision
  // TODO(Jacob) OverlapCollider - touching or inside

 private:
  static class CollisionsModule *collisionsModule;
  friend class CollisionsModule;
};
}  // namespace Isetta
