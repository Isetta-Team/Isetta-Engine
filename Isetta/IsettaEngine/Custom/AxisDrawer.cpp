/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/AxisDrawer.h"
#include "Core/Debug/DebugDraw.h"
#include "Scene/Transform.h"

namespace Isetta {

void AxisDrawer::Update() {
  // DebugDraw::Axis(GetTransform().GetLocalToWorldMatrix(), Color::red,
                  // Color::green, Color::blue, 2, 0, 2, false);

  DebugDraw::AxisSphere(GetTransform().GetLocalToWorldMatrix());
  Math::Matrix4 mat = Math::Matrix4::identity;
  mat.SetCol(3, GetTransform().GetWorldPos(), 1);
  DebugDraw::Axis(mat, Color::red, Color::green, Color::blue, 2, 0, 3, false);
}
}  // namespace Isetta
