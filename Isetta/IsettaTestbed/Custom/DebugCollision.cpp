/*
 * Copyright (c) 2018 Isetta
 */
#include "DebugCollision.h"

namespace Isetta {
// Register collision callbacks
void DebugCollision::OnEnable() {
  // Get all colliders on this entity
  colliders = entity->GetComponents<Collider>();
  // CollisionHandler is responsible for callback events
  handler = entity->GetComponent<CollisionHandler>();

  // Register callback to OnEnter
  //  BIND_1 binds this object to the member method OnCollisionEnter
  enterHandle =
      handler->RegisterOnEnter(BIND_1(DebugCollision::OnCollisionEnter, this));
  // Register callback to OnExit
  exitHandle =
      handler->RegisterOnExit(BIND_1(DebugCollision::OnCollisionExit, this));
}

// Unregister collision callbacks
void DebugCollision::OnDisable() {
  handler->UnregisterOnEnter(enterHandle);
  handler->UnregisterOnExit(exitHandle);
}

// Set all colliders on entity to red on intersection
void DebugCollision::OnCollisionEnter(Collider* const col) {
  collisions++;
  for (auto& c : colliders) {
    // debugColor is the color of Collider in _Editor
    c->debugColor = Color::red;
  }
}

// Set all colliders on entity to green if no intersections
void DebugCollision::OnCollisionExit(Collider* const other) {
  collisions--;
  if (collisions == 0) {
    for (auto& c : colliders) {
      c->debugColor = Color::green;
    }
  }
}
}  // namespace Isetta
