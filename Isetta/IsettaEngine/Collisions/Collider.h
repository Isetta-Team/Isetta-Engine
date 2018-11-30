/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collisions/AABB.h"
#include "Core/Color.h"
#include "Core/Math/Vector3.h"
#include "Scene/Component.h"
#include "Scene/Transform.h"

namespace Isetta::Math {
class Ray;
}

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

DEFINE_COMPONENT(Collider, Component, false)
public:
bool isTrigger = false;
Math::Vector3 center;
Color debugColor = Color::green;
float mass = 1;

// TODO(Jacob) virtual Math::Vector3 ClosestPoint(Math::Vector3 point) = 0;
// TODO(Jacob) Math::Vector3 ClosestPointOnAABB(Math::Vector3 point);
virtual bool Raycast(const class Ray& ray, class RaycastHit* const hitInfo,
                     float maxDistance = 0) = 0;

Math::Vector3 GetWorldCenter() const {
  return transform->WorldPosFromLocalPos(center);
}

void Start() override;
void OnEnable() override;
void OnDisable() override;

virtual AABB GetFatAABB() = 0;
virtual AABB GetAABB() = 0;

private:
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
friend class CollisionSolverModule;

protected:
inline static float fatFactor = 0.2f;

Collider(const Math::Vector3& center) : center{center} {}
Collider(const bool trigger = false,
         const Math::Vector3& center = Math::Vector3::zero)
    : center{center}, isTrigger{trigger} {}
virtual ~Collider() = default;

friend class BoxCollider;
friend class SphereCollider;
friend class CapsuleCollider;

enum class ColliderType { BOX, SPHERE, CAPSULE };
virtual ColliderType GetType() const = 0;

virtual bool Intersection(Collider* other) = 0;
void RaycastHitCtor(class RaycastHit* hitInfo, float distance,
                    const Math::Vector3& point, const Math::Vector3& normal);
DEFINE_COMPONENT_END(Collider, Component)
}  // namespace Isetta
