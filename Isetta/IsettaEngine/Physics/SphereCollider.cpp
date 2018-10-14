/*
 * Copyright (c) 2018 Isetta
 */
#include "Physics/SphereCollider.h"
#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/PhysicsModule.h"

#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Matrix4.h"
#include "Scene/Transform.h"

namespace Isetta {
void SphereCollider::Update() {
  DebugDraw::AxisSphere(
      Math::Matrix4::Translate(GetTransform().GetWorldPos() + center) *
          Math::Matrix4::Scale(
              Math::Vector3{radius * GetTransform().GetWorldScale().Max()}),
      debugColor, debugColor, debugColor);
}

bool SphereCollider::Raycast(const Ray& ray, RaycastHit* const hitInfo,
                             float maxDistance) {
  Math::Vector3 to = ray.GetOrigin() - (center + GetTransform().GetWorldPos());
  float b = Math::Vector3::Dot(to, ray.GetDirection());
  float c = Math::Vector3::Dot(to, to) - GetWorldRadius();
  if (c > 0.0f && b > 0.0f) {
    return false;
  }
  float discr = b * b - c;
  if (discr < 0.0f) {
    return false;
  }
  t = -b - Math::Util::Sqrt(discr);
  if (t < 0.0f) {
    return 0.0f;
  }
  RaycastHitCtor(hitInfo, t, ray.GetPoint(t), -ray.GetDirection());
  return true;
}

INTERSECTION_TEST(SphereCollider)
}  // namespace Isetta
