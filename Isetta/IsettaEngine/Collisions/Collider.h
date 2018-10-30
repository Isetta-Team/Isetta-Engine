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

CREATE_COMPONENT_BEGIN(Collider, Component, false)
public:
enum class Properties { IS_STATIC, IS_TRIGGER };
inline void SetProperties(Properties attr, bool value) {
  properties.set(static_cast<int>(attr), value);
}
inline bool GetProperties(Properties attr) const {
  return properties.test(static_cast<int>(attr));
}

  Math::Vector3 center;
  Color debugColor = Color::green;

  // TODO(Jacob) virtual Math::Vector3 ClosestPoint(Math::Vector3 point) = 0;
  // TODO(Jacob) Math::Vector3 ClosestPointOnAABB(Math::Vector3 point);
virtual bool Raycast(const class Ray& ray, class RaycastHit* const hitInfo,
                     float maxDistance = 0) = 0;

inline Math::Vector3 GetWorldCenter() const {
  return center + GetTransform()->GetWorldPos();
}

  void Start() override;
void OnEnable() override;
  void OnDisable() override;

// TODO(YIDI): Actually implement this for each collider
virtual AABB GetFatAABB() {
  return AABB{Math::Vector3::zero, Math::Vector3::one * (1 + fatFactor)};
}

virtual AABB GetAABB() { return AABB{Math::Vector3::zero, Math::Vector3::one}; }

// TODO(YIDI): DebugOnly
void AddToBVTree();

private:
std::bitset<2> properties;
  int hierchyHandle;
  class CollisionHandler* handler{nullptr};

  inline class CollisionHandler* GetHandler() { return handler; }
  inline void SetHandler(class CollisionHandler* const h) { handler = h; }
  void FindHandler();

static class CollisionsModule* collisionsModule;
friend class CollisionsModule;
friend class CollisionHandler;

protected:
static float fatFactor;

Collider(const Math::Vector3& center) : center{center}, properties{0b00} {}
Collider(bool isStatic = false, bool isTrigger = false,
         const Math::Vector3& center = Math::Vector3::zero)
    : center{center} {
  properties[(int)Properties::IS_STATIC] = isStatic;
  properties[(int)Properties::IS_TRIGGER] = isTrigger;
}
virtual ~Collider() = default;

enum class ColliderType { BOX, SPHERE, CAPSULE };
friend class BoxCollider;
friend class SphereCollider;
friend class CapsuleCollider;
virtual ColliderType GetType() const = 0;

virtual bool Intersection(Collider* const other) = 0;
void RaycastHitCtor(class RaycastHit* const hitInfo, float distance,
                    const Math::Vector3& point, const Math::Vector3& normal);
CREATE_COMPONENT_END(Collider, Component)
}  // namespace Isetta
