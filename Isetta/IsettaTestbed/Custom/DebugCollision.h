/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
namespace Isetta {
DEFINE_COMPONENT(DebugCollision, Component, true)
private:
int collisions = 0;
int enterHandle, exitHandle;
Array<class Collider*> colliders;
class CollisionHandler* handler;

public:
void OnEnable() override;
void OnDisable() override;
void OnCollisionEnter(class Collider* const other);
void OnCollisionExit(class Collider* const other);
DEFINE_COMPONENT_END(DebugCollision, Component)
}  // namespace Isetta
