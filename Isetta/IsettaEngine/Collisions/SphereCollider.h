/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/Collider.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(SphereCollider, Collider)
private:
void Update() override;

protected:
const ColliderType GetType() const override {
  return Collider::ColliderType::SPHERE;
}

public:
float radius;

explicit SphereCollider(float radius = 0.5) : Collider{}, radius{radius} {}
SphereCollider(const Math::Vector3& center, float radius = 0.5)
    : Collider{center}, radius{radius} {}
SphereCollider(bool isStatic, bool isTrigger, const Math::Vector3& center,
               float radius = 0.5)
    : Collider{isStatic, isTrigger, center}, radius{radius} {}

bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) override;

inline float GetWorldRadius() const {
  return radius * GetTransform()->GetWorldScale().Max();
}
bool Intersection(Collider* const other) override;
CREATE_COMPONENT_END(SphereCollider, Collider)
}  // namespace Isetta
