/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/AABB.h"
#include "Core/Color.h"
#include "Core/Math/Vector3.h"
#include "Scene/Component.h"
#include "Scene/Transform.h"

namespace Isetta {
#define INTERSECTION_TEST(TYPE)                             \
  bool TYPE::Intersection(Collider* const other) {          \
    switch (other->GetType()) {                             \
      case Collider::ColliderType::BOX:                     \
        return collisionsModule->Intersection(              \
            *this, dynamic_cast<BoxCollider&>(*other));     \
        break;                                              \
      case Collider::ColliderType::SPHERE:                  \
        return collisionsModule->Intersection(              \
            *this, dynamic_cast<SphereCollider&>(*other));  \
        break;                                              \
      case Collider::ColliderType::CAPSULE:                 \
        return collisionsModule->Intersection(              \
            *this, dynamic_cast<CapsuleCollider&>(*other)); \
        break;                                              \
    };                                                      \
  }

class Collider : public Component {
 public:
  enum class Attributes { IS_STATIC, IS_TRIGGER };
  inline void SetAttribute(Attributes attr, bool value) {
    attributes.set(static_cast<int>(attr), value);
  }
  inline bool GetAttribute(Attributes attr) const {
    return attributes.test(static_cast<int>(attr));
  }

  Math::Vector3 center;  // TODO(JACOB) remove

  // virtual Math::Vector3 ClosestPoint(Math::Vector3 point) = 0;
  // Math::Vector3 ClosestPointOnAABB(Math::Vector3 point);
  virtual bool Raycast(const class Ray& ray, class RaycastHit* const hitInfo,
                       float maxDistance = 0) = 0;

  inline Math::Vector3 GetWorldCenter() const {
    return center + GetTransform().GetWorldPos();
  }

  void OnDisable() override;
  void OnEnable() override;

 private:
  std::bitset<2> attributes;

 protected:
  AABB* bounding;

  Color debugColor = Color::green;

  Collider(const Math::Vector3& center) : center{center} {
    attributes[(int)Attributes::IS_STATIC] = 0;
    attributes[(int)Attributes::IS_TRIGGER] = 0;
  }
  Collider(bool isStatic = false, bool isTrigger = false,
           const Math::Vector3& center = Math::Vector3::zero)
      : center{center} {
    attributes[(int)Attributes::IS_STATIC] = isStatic;
    attributes[(int)Attributes::IS_TRIGGER] = isTrigger;
  }
  virtual ~Collider() = default;

  enum class ColliderType { BOX, SPHERE, CAPSULE };
  friend class BoxCollider;
  friend class SphereCollider;
  friend class CapsuleCollider;
  virtual const ColliderType GetType() const = 0;

  virtual bool Intersection(Collider* const other) = 0;
  void RaycastHitCtor(class RaycastHit* const hitInfo, float distance,
                      const Math::Vector3& point, const Math::Vector3& normal);

  static class CollisionsModule* collisionsModule;
  friend class CollisionsModule;
};
}  // namespace Isetta
