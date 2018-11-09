/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/Collider.h"

namespace Isetta::Math {
class Matrix4;
}

namespace Isetta {
BEGIN_COMPONENT(CapsuleCollider, Collider, false)
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

CapsuleCollider(float radius = 0.5, float height = 1,
                Direction direction = Direction::Y_AXIS)
    : Collider{}, radius{radius}, height{height}, direction{direction} {}
CapsuleCollider(const Math::Vector3& center, float radius = 0.5,
                float height = 1, Direction direction = Direction::Y_AXIS)
    : Collider{center}, radius{radius}, height{height}, direction{direction} {}
CapsuleCollider(bool isStatic, bool isTrigger, const Math::Vector3& center,
                float radius = 0.5, float height = 1,
                Direction direction = Direction::Y_AXIS)
    : Collider{isStatic, isTrigger, center},
      radius{radius},
      height{height},
      direction{direction} {}

float GetWorldCapsule(Math::Matrix4* rotation, Math::Matrix4* scale) const;

bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) final;

inline float GetWorldRadius() const {
  switch (direction) {
    case Direction::X_AXIS:
      return Math::Util::Max(GetTransform()->GetWorldScale().y,
                             GetTransform()->GetWorldScale().z);
    case Direction::Y_AXIS:
      return Math::Util::Max(GetTransform()->GetWorldScale().x,
                             GetTransform()->GetWorldScale().z);
    case Direction::Z_AXIS:
      return Math::Util::Max(GetTransform()->GetWorldScale().x,
                             GetTransform()->GetWorldScale().y);
  }
}
inline float GetWorldHeight() const {
  switch (direction) {
    case Direction::X_AXIS:
      return GetTransform()->GetWorldScale().x;
    case Direction::Y_AXIS:
      return GetTransform()->GetWorldScale().y;
    case Direction::Z_AXIS:
      return GetTransform()->GetWorldScale().z;
  }
}

AABB GetFatAABB() final;
AABB GetAABB() final;

bool Intersection(Collider* const other) final;
END_COMPONENT(CapsuleCollider, Collider)
}  // namespace Isetta
