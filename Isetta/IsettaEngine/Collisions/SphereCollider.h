/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/Collider.h"

namespace Isetta {
DEFINE_COMPONENT(SphereCollider, Collider, false)
private:
#if _EDITOR
void Update() override;
#endif

protected:
ColliderType GetType() const final { return ColliderType::SPHERE; }

public:
float radius;

explicit SphereCollider(const float radius = 0.5)
    : Collider{}, radius{radius} {}

explicit SphereCollider(const Math::Vector3& center, const float radius = 0.5)
    : Collider{center}, radius{radius} {}
SphereCollider(bool trigger, const Math::Vector3& center,
               float radius = 0.5)
    : Collider{trigger, center}, radius{radius} {}

bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) final;

float GetWorldRadius() const {
  return radius * transform->GetWorldScale().Max();
}

AABB GetFatAABB() final;
AABB GetAABB() final;

bool Intersection(Collider* const other) final;
DEFINE_COMPONENT_END(SphereCollider, Collider)
}  // namespace Isetta
