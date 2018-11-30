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
Math::Vector3 size;

BoxCollider(const Math::Vector3& center = Math::Vector3::zero,
            const Math::Vector3& size = Math::Vector3::one)
    : Collider{center}, size{size} {}
BoxCollider(bool trigger, const Math::Vector3& center,
            const Math::Vector3& size = Math::Vector3::one)
    : Collider{trigger, center}, size{size} {}

bool Raycast(const class Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) final;

inline Math::Vector3 GetWorldSize() const {
  return Math::Vector3::Scale(size, transform->GetWorldScale());
}
inline Math::Vector3 GetWorldExtents() const { return 0.5f * GetWorldSize(); }

AABB GetFatAABB() final;
AABB GetAABB() final;

bool Intersection(Collider* const other) final;
DEFINE_COMPONENT_END(BoxCollider, Collider)
}  // namespace Isetta
