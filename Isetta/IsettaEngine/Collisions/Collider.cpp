/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/Collider.h"
#include "Collisions/CollisionHandler.h"
#include "Collisions/CollisionsModule.h"

#include "Collisions/RaycastHit.h"
#include "Core/Geometry/Ray.h"
#include "Scene/Entity.h"

namespace Isetta {
CollisionsModule* Collider::collisionsModule{nullptr};

void Collider::Start() {
  if (!handler) FindHandler();
}

void Collider::OnEnable() {
  collisionsModule->bvTree.AddCollider(this);
  // TODO(Yidi) + TODO(Jacob)
  // hierarchyHandle =
  // entity->OnHierarchyChange.Register(std::bind(&Collider::FindHandler,this));
}

void Collider::OnDisable() {
  collisionsModule->bvTree.RemoveCollider(this);
  // TODO(Yidi) + TODO(Jacob)
  // entity->OnHierarchyChange.Unregister(hierarchyHandle);
}
void Collider::FindHandler() {
  handler = entity->GetComponent<CollisionHandler>();
  Transform* parent = transform->GetParent();
  while (parent && !handler) {
    handler = parent->entity->GetComponent<CollisionHandler>();
    parent = parent->GetParent();
  }
}
void Collider::RaycastHitCtor(RaycastHit* const hitInfo, const float distance,
                              const Math::Vector3& point,
                              const Math::Vector3& normal) {
  *hitInfo = RaycastHit{this, distance, point, normal.Normalized()};
}
}  // namespace Isetta
