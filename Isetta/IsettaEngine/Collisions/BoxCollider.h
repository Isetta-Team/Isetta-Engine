/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/Collider.h"

namespace Isetta {
DEFINE_COMPONENT(BoxCollider, Collider, false)
private:
#if _EDITOR
void Update() override;
#endif

protected:
ColliderType GetType() const final { return ColliderType::BOX; }

public:
/**
 * @brief size of box before transform scale
 *
 */
Math::Vector3 size;

/**
 * @brief Construct a new Box Collider object
 *
 * @param center offset from transform position
 * @param size of box, scaled by transform scale
 */
BoxCollider(const Math::Vector3& center = Math::Vector3::zero,
            const Math::Vector3& size = Math::Vector3::one)
    : Collider{center}, size{size} {}
/**
 * @brief Construct a new Box Collider object
 *
 * @param trigger whether collidable or trigger
 * @param center offset from transform position
 * @param size of box, scaled by transform scale
 */
BoxCollider(bool trigger, const Math::Vector3& center,
            const Math::Vector3& size = Math::Vector3::one)
    : Collider{trigger, center}, size{size} {}

/**
 * @brief Cast ray at BoxCollider to determine RaycastHit.
 * [Warning] RaycastHit.normal does NOT return correct normal
 *
 * @param ray which is being cast
 * @param hitInfo information associated with a hit of box and ray
 * @param maxDistance that the ray will check against
 * @return true ray intersected collider
 */
bool Raycast(const class Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) final;

/**
 * @brief Get the World Size
 *
 * @return Math::Vector3 size of box scaled with transform
 */
inline Math::Vector3 GetWorldSize() const {
  return Math::Vector3::Scale(size, transform->GetWorldScale());
}
/**
 * @brief Get the World Extents object
 *
 * @return Math::Vector3 half-size of box scaled with transform
 */
inline Math::Vector3 GetWorldExtents() const { return 0.5f * GetWorldSize(); }

AABB GetFatAABB() final;
AABB GetAABB() final;

/**
 * @brief Intersection test with other collider
 *
 * @param other collider to check against
 * @return true collider is intersecting with box
 */
bool Intersection(Collider* const other) final;
DEFINE_COMPONENT_END(BoxCollider, Collider)
}  // namespace Isetta
