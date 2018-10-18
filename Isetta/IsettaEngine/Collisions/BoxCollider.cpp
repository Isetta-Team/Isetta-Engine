/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/CollisionsModule.h"
#include "Collisions/SphereCollider.h"

#include "Collisions/Ray.h"
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
