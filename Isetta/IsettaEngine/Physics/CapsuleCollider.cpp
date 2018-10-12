/*
 * Copyright (c) 2018 Isetta
 */
#include "Physics/CapsuleCollider.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Matrix4.h"
#include "Scene/Transform.h"

namespace Isetta {
void CapsuleCollider::Update() {
  Math::Matrix4 scale;
  Math::Matrix4 rotation;
  TransformUtility(rotation, scale);
  DebugDraw::WireCapsule(
      Math::Matrix4::Translate(GetTransform().GetWorldPos() + center) *
          rotation * scale,
      radius, height, Color::green);
}
float CapsuleCollider::TransformUtility(Math::Matrix4& rotation,
                                        Math::Matrix4 scale) const {
  rotation = (Math::Matrix4)(GetTransform().GetWorldRot());
  float max;
  switch (direction) {
    case Direction::X_AXIS:
      rotation *= Math::Matrix4::zRot90;
      max = Math::Util::Max(GetTransform().GetWorldScale().y,
                            GetTransform().GetWorldScale().z);
      scale = Math::Matrix4::Scale(
          Math::Vector3{GetTransform().GetWorldScale().x, max, max});
      break;
    case Direction::Y_AXIS:
      rotation *= Math::Matrix4::identity;
      max = Math::Util::Max(GetTransform().GetWorldScale().x,
                            GetTransform().GetWorldScale().z);
      scale = Math::Matrix4::Scale(
          Math::Vector3{max, GetTransform().GetWorldScale().y, max});
      break;
    case Direction::Z_AXIS:
      rotation *= Math::Matrix4::xRot90;
      max = Math::Util::Max(GetTransform().GetWorldScale().x,
                            GetTransform().GetWorldScale().y);
      scale = Math::Matrix4::Scale(
          Math::Vector3{max, max, GetTransform().GetWorldScale().z});
      break;
  }
  return max;
}
}  // namespace Isetta
