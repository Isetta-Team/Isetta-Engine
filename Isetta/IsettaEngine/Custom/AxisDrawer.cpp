/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/AxisDrawer.h"
#include "Core/Debug/DebugDraw.h"
#include "Scene/Transform.h"

namespace Isetta {

void AxisDrawer::Update() {
  DebugDraw::Axis(GetTransform().GetLocalToWorldMatrix());
}
}  // namespace Isetta
