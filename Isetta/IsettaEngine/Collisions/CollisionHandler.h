/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
// #include <unordered_map>
#include "Core/DataStructures/Delegate.h"
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(CollisionHandler, Component, true)
private:
Delegate<class Collider*> onEnter, onStay, onExit;

static void SetColliderHandler(Transform* transform,
                               const Action<Collider* const>& action);

void OnCollisionEnter(class Collider* const col);
void OnCollisionStay(class Collider* const col);
void OnCollisionExit(class Collider* const col);

friend class CollisionsModule;

public:
CollisionHandler() = default;

void OnEnable() override;
void OnDisable() override;

/**
 * @brief Register callback when collider enters attached entity's colliders or
 * child colliders
 *
 * @param callback that occurs on event
 * @return U16 handle to unregister the callback
 */
U64 RegisterOnEnter(const Action<class Collider* const>& callback);
/**
 * @brief Unregister on enter callback
 *
 * @param handle to unregister
 */
void UnregisterOnEnter(U64& handle);

/**
 * @brief Register callback when collider remains in attached entity's colliders
 * or child colliders, occurs each FixedUpdated
 *
 * @param callback that occurs on event
 * @return U16 handle to unregister the callback
 */
U64 RegisterOnStay(const Action<class Collider* const>& callback);
/**
 * @brief Unregister on stay callback
 *
 * @param handle to unregister
 */
void UnregisterOnStay(U64& handle);

/**
 * @brief Register callback when collider exits attached entity's colliders or
 * child colliders
 *
 * @param callback that occurs on event
 * @return U16 handle to unregister the callback
 */
U64 RegisterOnExit(const Action<class Collider* const>& callback);
/**
 * @brief Unregister on exit callback
 *
 * @param handle to unregister
 */
void UnregisterOnExit(U64& handle);
DEFINE_COMPONENT_END(CollisionHandler, Component)
}  // namespace Isetta
