/*
 * Copyright (c) 2018 Isetta
 */
#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/PhysicsModule.h"
#include "Physics/SphereCollider.h"

#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Matrix4.h"
#include "Scene/Transform.h"

namespace Isetta {
void BoxCollider::Update() {
  DebugDraw::WireCube(
      Math::Matrix4::Translate(GetTransform().GetWorldPos() + center) *
          Math::Matrix4::Scale(
              Math::Vector3::Scale(GetTransform().GetWorldScale(), size)) *
          (Math::Matrix4)GetTransform().GetWorldRot(),
      debugColor);
}

bool BoxCollider::Raycast(const Ray& ray, RaycastHit* const hitInfo,
                          float maxDistance) {
  return false;
}

INTERSECTION_TEST(BoxCollider)
}  // namespace Isetta
