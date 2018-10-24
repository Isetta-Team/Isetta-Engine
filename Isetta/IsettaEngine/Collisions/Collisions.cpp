/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/Collisions.h"

#include "Collisions/CollisionsModule.h"

namespace Isetta {
CollisionsModule* Collisions::collisionsModule{nullptr};

bool Collisions::Raycast(const Ray& ray, RaycastHit* const hitInfo,
                         float maxDistance) {
  return collisionsModule->Raycast(ray, hitInfo, maxDistance);
}
}  // namespace Isetta
