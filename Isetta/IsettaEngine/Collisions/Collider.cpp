/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/Collider.h"
#include "Collisions/CollisionHandler.h"
#include "Collisions/CollisionsModule.h"

#include "Collisions/Ray.h"
#include "Scene/Entity.h"

namespace Isetta {
CollisionsModule* Collider::collisionsModule{nullptr};

void Collider::Start() {
  if (!handler) FindHandler();
  // TODO(Yidi + Jacob)
  // hierarchyHandle =
  // entity->OnHierarchyChange.Register(std::bind(&Collider::FindHandler,this));
}

void Collider::OnEnable() {
  // TODO(Yidi) add AABB
  collisionsModule->colliders.insert(this);
  collisionsModule->bvTree.AddCollider(this);
  // TODO(Yidi + Jacob)
  // entity->OnHierarchyChange.Unregister(hierarchyHandle);
}

void Collider::OnDisable() {
  // TODO(Yidi) remove AABB
  collisionsModule->colliders.erase(this);
  collisionsModule->bvTree.RemoveCollider(this);
}

AABB Collider::GetFatAABB() {
  AABB aabb = GetAABB();
  aabb.ExpandPercentage(fatFactor);
  return aabb;
}

AABB Collider::GetAABB() {
   return AABB{Math::Vector3::zero, Math::Vector3::one}; 
}

void Collider::FindHandler() {
  handler = entity->GetComponent<CollisionHandler>();
  Transform* parent = GetTransform()->GetParent();
  while (parent && !handler) {
    handler = parent->GetEntity()->GetComponent<CollisionHandler>();
    parent = parent->GetParent();
  }
}
void Collider::RaycastHitCtor(RaycastHit* const hitInfo, const float distance,
                              const Math::Vector3& point,
                              const Math::Vector3& normal) {
  *hitInfo = RaycastHit{this, distance, point, normal.Normalized()};
}

}  // namespace Isetta
