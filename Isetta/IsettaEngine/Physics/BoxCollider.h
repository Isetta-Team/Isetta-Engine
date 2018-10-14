/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Physics/Collider.h"

namespace Isetta {
class BoxCollider : public Collider {
 protected:
  const ColliderType GetType() const override {
    return Collider::ColliderType::BOX;
  }

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

  void Update() override;

  bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
               float maxDistance = 0) override;

  inline Math::Vector3 GetWorldSize() const {
    return Math::Vector3::Scale(size, GetTransform().GetWorldScale());
  }
  bool Intersection(Collider* const other) override;
};
}  // namespace Isetta
