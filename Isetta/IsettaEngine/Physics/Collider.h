/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Color.h"
#include "Core/Math/Vector3.h"
#include "Physics/AABB.h"
#include "Physics/Ray.h"
#include "Scene/Component.h"
#include "Scene/Transform.h"

namespace Isetta {
#define INTERSECTION_TEST(TYPE)                             \
  bool TYPE::Intersection(Collider* const other) {          \
    switch (other->GetType()) {                             \
      case Collider::ColliderType::BOX:                     \
        return physicsModule->Intersection(                 \
            *this, dynamic_cast<BoxCollider&>(*other));     \
        break;                                              \
      case Collider::ColliderType::SPHERE:                  \
        return physicsModule->Intersection(                 \
            *this, dynamic_cast<SphereCollider&>(*other));  \
        break;                                              \
      case Collider::ColliderType::CAPSULE:                 \
        return physicsModule->Intersection(                 \
            *this, dynamic_cast<CapsuleCollider&>(*other)); \
        break;                                              \
    };                                                      \
  }

class Collider : public Component {
 public:
  bool isTrigger, isStatic;  // TODO(Jacob) make private, getter/setter
  Math::Vector3 center;      // TODO(JACOB) remove

  // virtual Math::Vector3 ClosestPoint(Math::Vector3 point) = 0;
  // Math::Vector3 ClosestPointOnAABB(Math::Vector3 point);
  virtual bool Raycast(const Ray& ray, RaycastHit* const hitInfo,
                       float maxDistance = 0) = 0;

  inline Math::Vector3 GetWorldCenter() const {
    return center + GetTransform().GetWorldPos();
  }

  void OnDisable() override;
  void OnEnable() override;

 private:
 protected:
  AABB* bounding;

  Color debugColor = Color::green;

  Collider(const Math::Vector3& center)
      : isStatic{false}, isTrigger{false}, center{center} {}
  Collider(bool isStatic = false, bool isTrigger = false,
           const Math::Vector3& center = Math::Vector3::zero)
      : isStatic{isStatic}, isTrigger{isTrigger}, center{center} {}
  virtual ~Collider() = default;

  enum class ColliderType { BOX, SPHERE, CAPSULE };
  friend class BoxCollider;
  friend class SphereCollider;
  friend class CapsuleCollider;
  virtual const ColliderType GetType() const = 0;

  virtual bool Intersection(Collider* const other) = 0;
  void RaycastHitCtor(class RaycastHit* const hitInfo, float distance,
                      const Math::Vector3& point, const Math::Vector3& normal);

  static class PhysicsModule* physicsModule;
  friend class PhysicsModule;
};
}  // namespace Isetta
