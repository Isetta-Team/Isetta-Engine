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
enum class Direction { X_AXIS, Y_AXIS, Z_AXIS };

/**
 * @brief radius before scaled by transform
 * @brief height before scaled by transform
 *
 */
float radius, height;
/**
 * @brief direction which the capsule initially lay (prior to transform
 * rotation)
 *
 */
Direction direction;

/**
 * @brief Construct a new Capsule Collider object
 *
 * @param radius scaled by max transform scale not in direction
 * @param height scaled by transform scale in direction
 * @param direction which the capsule initially lay (prior to transform
 * rotation)
 */
CapsuleCollider(float radius = 0.5, float height = 2,
                Direction direction = Direction::Y_AXIS)
    : Collider{}, radius{radius}, height{height}, direction{direction} {}
/**
 * @brief Construct a new Capsule Collider object
 *
 * @param center offset from transform position
 * @param radius scaled by max transform scale not in direction
 * @param height scaled by transform scale in direction
 * @param direction which the capsule initially lay (prior to transform
 * rotation)
 */
CapsuleCollider(const Math::Vector3& center, float radius = 0.5,
                float height = 2, Direction direction = Direction::Y_AXIS)
    : Collider{center}, radius{radius}, height{height}, direction{direction} {}
/**
 * @brief Construct a new Capsule Collider object
 *
 * @param trigger whether collidable or trigger
 * @param center offset from transform position
 * @param radius scaled by max transform scale not in direction
 * @param height scaled by transform scale in direction
 * @param direction which the capsule initially lay (prior to transform
 * rotation)
 */
CapsuleCollider(bool trigger, const Math::Vector3& center, float radius = 0.5,
                float height = 2, Direction direction = Direction::Y_AXIS)
    : Collider{trigger, center},
      radius{radius},
      height{height},
      direction{direction} {}

/**
 * @brief Get the World Capsule object
 *
 * @param rotation transform rotation with direction axis factored in
 * @param scale transform scale multiplied with direction axis multiplied by
 * height and the other multiplied by radius
 * @return float max transform scale not in direction
 */
float GetWorldCapsule(Math::Matrix4* rotation, Math::Matrix4* scale) const;

/**
 * @brief Cast ray at CapsuleCollider to determine RaycastHit.
 *
 * @param ray which is being cast
 * @param hitInfo information associated with a hit of box and ray
 * @param maxDistance that the ray will check against
 * @return true ray intersected collider
 */
bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
             float maxDistance = 0) final;

/**
 * @brief Get the World Radius
 *
 * @return float max transform scale not in direction multiplied by radius
 */
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
/**
 * @brief Get the World Height object
 *
 * @return float transform scale in direction multiplied by height
 */
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
