/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_map>
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(CollisionHandler, Component, true)
private:
std::unordered_map<U16, Action<class Collider*>> onEnter, onStay, onExit;
U16 handles = 0;

static void SetColliderHandler(Transform* transform,
                               const Action<Collider* const>& action);

void OnCollisionEnter(class Collider* const col);
void OnCollisionStay(class Collider* const col);
void OnCollisionExit(class Collider* const col);

friend class CollisionsModule;

protected:
void OnCollisionCallback(
    const std::unordered_map<U16, Action<class Collider*>>& callbacks,
    class Collider* const col);

public:
CollisionHandler() = default;

void OnEnable() override;
void OnDisable() override;

U16 RegisterOnEnter(const Action<class Collider* const>& action);
void UnregisterOnEnter(U16 handle);
U16 RegisterOnStay(const Action<class Collider* const>& action);
void UnregisterOnStay(U16 handle);
U16 RegisterOnExit(const Action<class Collider* const>& action);
void UnregisterOnExit(U16 handle);
DEFINE_COMPONENT_END(CollisionHandler, Component)
}  // namespace Isetta
