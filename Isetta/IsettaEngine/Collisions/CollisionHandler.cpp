/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/CollisionHandler.h"

#include "Collisions/Collider.h"
#include "Core/DataStructures/Array.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace Isetta {
void CollisionHandler::OnEnable() {
  Array<Collider*> colliders = entity->GetComponents<Collider>();
  for (int i = 0; i < colliders.Size(); ++i) {
    colliders[i]->SetHandler(this);
  }
  Action<Collider*> action = [&](Collider* c) { c->SetHandler(this); };
  for (auto& t : *transform) {
    SetColliderHandler(t, action);
  }
}
void CollisionHandler::OnDisable() {
  CollisionHandler* handler = nullptr;
  Transform* parent = transform->GetParent();
  while (parent && !handler) {
    handler = parent->entity->GetComponent<CollisionHandler>();
    parent = parent->GetParent();
  }
  Array<Collider*> colliders = entity->GetComponents<Collider>();
  for (int i = 0; i < colliders.Size(); ++i) {
    colliders[i]->SetHandler(this);
  }
  Action<Collider*> action = [&](Collider* c) { c->SetHandler(handler); };
  for (auto& t : *transform) {
    SetColliderHandler(t, action);
  }
}
void CollisionHandler::SetColliderHandler(
    Transform* transform, const Action<Collider* const>& action) {
  Entity* e = transform->entity;
  if (!e->GetComponent<CollisionHandler>()) {
    for (auto& t : *transform) {
      SetColliderHandler(t, action);
    }
    Collider* collider = e->GetComponent<Collider>();
    if (collider) {
      action(collider);
    }
  }
}
void CollisionHandler::OnCollisionEnter(Collider* const col) {
  onEnter.Invoke(col);
}
void CollisionHandler::OnCollisionStay(Collider* const col) {
  onStay.Invoke(col);
}
void CollisionHandler::OnCollisionExit(Collider* const col) {
  onExit.Invoke(col);
}

U64 CollisionHandler::RegisterOnEnter(
    const Action<class Collider* const>& callback) {
  return onEnter.Subscribe(callback);
}
void CollisionHandler::UnregisterOnEnter(U64& handle) {
  onEnter.Unsubscribe(handle);
}

U64 CollisionHandler::RegisterOnStay(
    const Action<class Collider* const>& callback) {
  return onStay.Subscribe(callback);
}
void CollisionHandler::UnregisterOnStay(U64& handle) {
  onStay.Unsubscribe(handle);
}

U64 CollisionHandler::RegisterOnExit(
    const Action<class Collider* const>& callback) {
  return onExit.Subscribe(callback);
}
void CollisionHandler::UnregisterOnExit(U64& handle) {
  onExit.Unsubscribe(handle);
}
}  // namespace Isetta
