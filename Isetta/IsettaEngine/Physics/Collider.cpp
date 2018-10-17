/*
 * Copyright (c) 2018 Isetta
 */
#include "Physics/Collider.h"
#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/PhysicsModule.h"
#include "Physics/SphereCollider.h"

#include "Physics/Ray.h"
#include "Scene/Entity.h"

namespace Isetta {
PhysicsModule* Collider::physicsModule{nullptr};
void Collider::OnEnable() {
  // physicsModule->tree.Add(bounding);
  // TODO(Jacob) remove
  // if (!isStatic) physicsModule->dynamicColliders.push_back(this);
  physicsModule->colliders.push_back(this);
}
void Collider::OnDisable() {
  // TODO(Jacob) remove this element
  /*physicsModule->colliders.erase();*/
}
void Collider::RaycastHitCtor(RaycastHit* const hitInfo, float distance,
                              const Math::Vector3& point,
                              const Math::Vector3& normal) {
  *hitInfo = std::move(RaycastHit{this, distance, normal, point});
}

}  // namespace Isetta
