/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_map>
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(CollisionHandler, Component, true)
private : std::unordered_map<U16, Action<class Collider*>> onEnter,
          onStay,
          onExit;
U16 handles;

static void ColliderHandle(Transform* t, const Action<Collider*>& action);

void OnCollisionEnter(class Collider* const col);
void OnCollisionStay(class Collider* const col);
void OnCollisionExit(class Collider* const col);

friend class CollisionsModule;

protected:
void OnCollisionCallback(
    const std::unordered_map<U16, Action<class Collider*>>& callbacks,
    class Collider* const col);

public:
void OnEnable() override;
void OnDisable() override;

U16 RegisterOnEnter(Action<class Collider*> action);
void UnregisterOnEnter(U16 handle);
U16 RegisterOnStay(Action<class Collider*> action);
void UnregisterOnStay(U16 handle);
U16 RegisterOnExit(Action<class Collider*> action);
void UnregisterOnExit(U16 handle);
CREATE_COMPONENT_END(CollisionHandler, Component)
}  // namespace Isetta
