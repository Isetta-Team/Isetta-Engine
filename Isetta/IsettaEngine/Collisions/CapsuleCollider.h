/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/Collider.h"

namespace Isetta::Math {
class Matrix4;
}

namespace Isetta {
DEFINE_COMPONENT(CapsuleCollider, Collider, false)
private:
#if _EDITOR
void Update() override;
#endif
// TODO(Jacob) duplicate code
bool RaycastSphere(const Math::Vector3& center, float radius, const Ray& ray,
                   RaycastHit* const hitInfo, float maxDistance);

protected:
ColliderType GetType() const final { return Collider::ColliderType::CAPSULE; }

public:
// TODO(Jacob) refactor into an outside enum class
enum class Direction { X_AXIS, Y_AXIS, Z_AXIS };

float radius, height;
Direction direction;

CapsuleCollider(float radius = 0.5, float height = 2,
                Direction direction = Direction::Y_AXIS)
    : Collider{}, radius{radius}, height{height}, direction{direction} {}
CapsuleCollider(const Math::Vector3& center, float radius = 0.5,
                float height = 2, Direction direction = Direction::Y_AXIS)
    : Collider{center}, radius{radius}, height{height}, direction{direction} {}
CapsuleCollider(bool trigger, const Math::Vector3& center, float radius = 0.5,
                float height = 2, Direction direction = Direction::Y_AXIS)
    : Collider{trigger, center},
      radius{radius},
      height{height},
      direction{direction} {}

float GetWorldCapsule(Math::Matrix4* rotation, Math::Matrix4* scale) const;

bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) final;

inline float GetWorldRadius() const {
  switch (direction) {
    case Direction::X_AXIS:
      return radius * Math::Util::Max(transform->GetWorldScale().y,
                                      transform->GetWorldScale().z);
    case Direction::Y_AXIS:
      return radius * Math::Util::Max(transform->GetWorldScale().x,
                                      transform->GetWorldScale().z);
    case Direction::Z_AXIS:
      return radius * Math::Util::Max(transform->GetWorldScale().x,
                                      transform->GetWorldScale().y);
  }
}
inline float GetWorldHeight() const {
  switch (direction) {
    case Direction::X_AXIS:
      return height * transform->GetWorldScale().x;
    case Direction::Y_AXIS:
      return height * transform->GetWorldScale().y;
    case Direction::Z_AXIS:
      return height * transform->GetWorldScale().z;
  }
}

AABB GetFatAABB() final;
AABB GetAABB() final;

bool Intersection(Collider* const other) final;
DEFINE_COMPONENT_END(CapsuleCollider, Collider)
}  // namespace Isetta
