/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/Collider.h"
#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/CollisionsModule.h"
#include "Collisions/SphereCollider.h"

#include "Collisions/Ray.h"
#include "Scene/Entity.h"

namespace Isetta {
CollisionsModule* Collider::collisionsModule{nullptr};
float Collider::fatFactor = 0.f;

void Collider::OnEnable() {
  // CollisionsModule->tree.Add(bounding);
  // TODO(Jacob) remove
  // if (!isStatic) CollisionsModule->dynamicColliders.push_back(this);
  collisionsModule->colliders.push_back(this);
}

void Collider::AddToBVTree() {
  collisionsModule->bvTree.AddCollider(this);
}

void Collider::OnDisable() {
  // TODO(Jacob) remove this element
  /*CollisionsModule->colliders.erase();*/
}
void Collider::RaycastHitCtor(RaycastHit* const hitInfo, float distance,
                              const Math::Vector3& point,
                              const Math::Vector3& normal) {
  *hitInfo = std::move(RaycastHit{this, distance, point, normal.Normalized()});
}

}  // namespace Isetta
