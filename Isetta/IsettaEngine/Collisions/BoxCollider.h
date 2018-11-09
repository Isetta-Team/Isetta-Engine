/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/Collider.h"

namespace Isetta {
BEGIN_COMPONENT(BoxCollider, Collider, false)
private:
#if _EDITOR
void Update() override;
#endif

protected:
ColliderType GetType() const final { return ColliderType::BOX; }

public:
Math::Vector3 size;

explicit BoxCollider(const Math::Vector3& size = Math::Vector3::one)
    : Collider{}, size{size} {}
BoxCollider(const Math::Vector3& center,
            const Math::Vector3& size = Math::Vector3::one)
    : Collider{center}, size{size} {}
BoxCollider(bool isStatic, bool isTrigger, const Math::Vector3& center,
            const Math::Vector3& size = Math::Vector3::one)
    : Collider{isStatic, isTrigger, center}, size{size} {}

bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) final;

inline Math::Vector3 GetWorldSize() const {
  return Math::Vector3::Scale(size, GetTransform()->GetWorldScale());
}
inline Math::Vector3 GetWorldExtents() const { return 0.5f * GetWorldSize(); }

AABB GetFatAABB() final;
AABB GetAABB() final;

bool Intersection(Collider* const other) final;
END_COMPONENT(BoxCollider, Collider)
}  // namespace Isetta
