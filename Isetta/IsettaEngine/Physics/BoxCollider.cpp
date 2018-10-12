/*
 * Copyright (c) 2018 Isetta
 */
#include "BoxCollider.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Matrix4.h"
#include "Scene/Transform.h"

namespace Isetta {
void BoxCollider::Update() {
  DebugDraw::WireCube(
      Math::Matrix4::Translate(GetTransform().GetWorldPos() + center) *
          Math::Matrix4::Scale(
              Math::Vector3::Scale(GetTransform().GetWorldScale(), size)) *
          Math::Matrix4(GetTransform().GetWorldRot()),
      Color::green);
}
}  // namespace Isetta
