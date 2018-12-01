/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Change the appearance of the collider on collision
 *
 */
namespace Isetta {
DEFINE_COMPONENT(DebugCollision, Component, false)
private:
int collisions = 0;
U64 enterHandle, exitHandle;
Array<class Collider*> colliders;
class CollisionHandler* handler;

public:
void OnEnable() override;
void OnDisable() override;
void OnCollisionEnter(class Collider* const other);
void OnCollisionExit(class Collider* const other);
DEFINE_COMPONENT_END(DebugCollision, Component)
}  // namespace Isetta
