/*
 * Copyright (c) 2018 Isetta
 */
#include "SphereCollider.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Matrix4.h"
#include "Scene/Transform.h"

namespace Isetta {
void SphereCollider::Update() {
  DebugDraw::AxisSphere(
      Math::Matrix4::Translate(GetTransform().GetWorldPos() + center) *
          Math::Matrix4::Scale(
              Math::Vector3{radius * GetTransform().GetWorldScale().Max()}),
      Color::green, Color::green, Color::green);
}
}  // namespace Isetta
