/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/Collider.h"

namespace Isetta {
BEGIN_COMPONENT(BoxCollider, Collider, false)
protected:
ColliderType GetType() const override { return ColliderType::BOX; }

public:
Math::Vector3 size;

explicit BoxCollider(const Math::Vector3& size = Math::Vector3::one)
    : Collider{}, size{size} {}

explicit BoxCollider(const Math::Vector3& center,
                     const Math::Vector3& size = Math::Vector3::one)
    : Collider{center}, size{size} {}

BoxCollider(const bool isStatic, const bool isTrigger,
            const Math::Vector3& center,
            const Math::Vector3& size = Math::Vector3::one)
    : Collider{isStatic, isTrigger, center}, size{size} {}

void Update() override;
AABB GetAABB() override;

bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) override;

Math::Vector3 GetWorldSize() const {
  return Math::Vector3::Scale(size, GetTransform()->GetWorldScale());
}

Math::Vector3 GetWorldExtents() const { return 0.5f * GetWorldSize(); }

bool Intersection(Collider* other) override;

END_COMPONENT(BoxCollider, Collider)
}  // namespace Isetta
