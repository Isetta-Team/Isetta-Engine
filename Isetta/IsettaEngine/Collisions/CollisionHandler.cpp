/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/CollisionHandler.h"

#include "Collisions/Collider.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace Isetta {
void CollisionHandler::OnEnable() {
  std::vector<Collider*> colliders = entity->GetComponents<Collider>();
  for (int i = 0; i < colliders.size(); i++) {
    colliders[i]->SetHandler(this);
  }
  GetTransform().ForChildren(
      std::bind(&CollisionHandler::ColliderHandle, std::placeholders::_1,
                [&](Collider* c) { c->SetHandler(this); }));
}
void CollisionHandler::OnDisable() {
  CollisionHandler* handler = nullptr;
  Transform* parent = GetTransform().GetParent();
  while (parent && !handler) {
    handler = parent->GetEntity()->GetComponent<CollisionHandler>();
    parent = parent->GetParent();
  }
  GetTransform().ForChildren(
      std::bind(&CollisionHandler::ColliderHandle, std::placeholders::_1,
                [&handler](Collider* c) { c->SetHandler(handler); }));
}
void CollisionHandler::ColliderHandle(Transform* t,
                                      const Action<Collider*>& action) {
  Entity* e = t->GetEntity();
  // TODO(Jacob) might need to optimize
  if (!e->GetComponent<CollisionHandler>()) {
    t->ForChildren(std::bind(&CollisionHandler::ColliderHandle,
                             std::placeholders::_1, action));
    Collider* col = e->GetComponent<Collider>();
    if (col) {
      action(col);
    }
  }
}
void CollisionHandler::OnCollisionEnter(Collider* const col) {
  OnCollisionCallback(onEnter, col);
}
void CollisionHandler::OnCollisionStay(Collider* const col) {
  OnCollisionCallback(onStay, col);
}
void CollisionHandler::OnCollisionExit(Collider* const col) {
  OnCollisionCallback(onExit, col);
}
void CollisionHandler::OnCollisionCallback(
    const std::unordered_map<U16, Action<Collider*>>& callbacks,
    Collider* const col) {
  for (const auto& callback : callbacks) {
    callback.second(col);
  }
}
U16 CollisionHandler::RegisterOnEnter(Action<Collider*> action) {
  int handle = handles++;
  onEnter.insert(std::make_pair(handle, action));
  return handle;
}
void CollisionHandler::UnregisterOnEnter(U16 handle) { onEnter.erase(handle); }
U16 CollisionHandler::RegisterOnStay(Action<Collider*> action) {
  int handle = handles++;
  onStay.insert(std::make_pair(handle, action));
  return handle;
}
void CollisionHandler::UnregisterOnStay(U16 handle) { onStay.erase(handle); }
U16 CollisionHandler::RegisterOnExit(Action<Collider*> action) {
  int handle = handles++;
  onExit.insert(std::make_pair(handle, action));
  return handle;
}
void CollisionHandler::UnregisterOnExit(U16 handle) { onExit.erase(handle); }
}  // namespace Isetta
