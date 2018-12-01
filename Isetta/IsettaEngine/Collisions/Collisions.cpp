/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/Collisions.h"

#include "Collisions/CollisionsModule.h"
#include "Collisions/RaycastHit.h"

namespace Isetta {
CollisionsModule *Collisions::collisionsModule{nullptr};

bool Collisions::Raycast(const Ray &ray, RaycastHit *const hitInfo,
                         float maxDistance) {
  return collisionsModule->Raycast(ray, hitInfo, maxDistance);
}
Array<RaycastHit> Collisions::RaycastAll(const Ray &ray, float maxDistance) {
  return collisionsModule->RaycastAll(ray, maxDistance);
}

bool Collisions::GetIgnoreLayerCollision(int layer1, int layer2) {
  return collisionsModule->GetIgnoreLayerCollision(layer1, layer2);
}
void Collisions::SetIgnoreLayerCollision(int layer1, int layer2,
                                         bool ignoreLayer) {
  collisionsModule->SetIgnoreLayerCollision(layer1, layer2, ignoreLayer);
}

void Collisions::SetIgnoreCollisions(Collider *const a, Collider *const b,
                                     bool ignore) {
  if (ignore) {
    collisionsModule->ignoreColliderPairs.insert({a, b});
  } else {
    collisionsModule->ignoreColliderPairs.erase({a, b});
  }
}
bool Collisions::GetIgnoreCollisions(Collider *const a, Collider *const b) {
  return collisionsModule->ignoreColliderPairs.find({a, b}) !=
         collisionsModule->ignoreColliderPairs.end();
}
}  // namespace Isetta
