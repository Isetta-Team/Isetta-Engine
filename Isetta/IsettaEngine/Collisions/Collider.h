/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <utility>
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

BEGIN_COMPONENT(Collider, Component, false)
public:
enum class Properties { IS_STATIC, IS_TRIGGER };

void SetProperties(Properties attr, const bool value) {
  properties.set(static_cast<int>(attr), value);
}
bool GetProperties(Properties attr) const {
  return properties.test(static_cast<int>(attr));
}

Math::Vector3 center;
Color debugColor = Color::green;

// TODO(Jacob) virtual Math::Vector3 ClosestPoint(Math::Vector3 point) = 0;
// TODO(Jacob) Math::Vector3 ClosestPointOnAABB(Math::Vector3 point);
virtual bool Raycast(const class Ray& ray, class RaycastHit* const hitInfo,
                     float maxDistance = 0) = 0;

Math::Vector3 GetWorldCenter() const {
  return center + GetTransform()->GetWorldPos();
}

void Start() override;
void OnEnable() override;
void OnDisable() override;

virtual AABB GetFatAABB() = 0;
virtual AABB GetAABB() = 0;

private:
std::bitset<2> properties;
int hierarchyHandle;
class CollisionHandler* handler{nullptr};

class CollisionHandler* GetHandler() const {
  return handler;
}
void SetHandler(class CollisionHandler* const h) { handler = h; }
void FindHandler();

static class CollisionsModule* collisionsModule;
friend class CollisionsModule;
friend class CollisionHandler;

protected:
inline static float fatFactor = 0.2f;

Collider(const Math::Vector3& center) : center{center}, properties{0b00} {}
Collider(const bool isStatic = false, const bool isTrigger = false,
         const Math::Vector3& center = Math::Vector3::zero)
    : center{center} {
  properties[static_cast<int>(Properties::IS_STATIC)] = isStatic;
  properties[static_cast<int>(Properties::IS_TRIGGER)] = isTrigger;
}
virtual ~Collider() = default;

friend class BoxCollider;
friend class SphereCollider;
friend class CapsuleCollider;

enum class ColliderType { BOX, SPHERE, CAPSULE };
virtual ColliderType GetType() const = 0;

virtual bool Intersection(Collider* other) = 0;
void RaycastHitCtor(class RaycastHit* hitInfo, float distance,
                    const Math::Vector3& point, const Math::Vector3& normal);
END_COMPONENT(Collider, Component)
}  // namespace Isetta
