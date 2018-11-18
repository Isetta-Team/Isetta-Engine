/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/AxisDrawer.h"
#include "Core/Debug/DebugDraw.h"
#include "Scene/Transform.h"

namespace Isetta {

void AxisDrawer::Update() {
  DebugDraw::Axis(transform->GetLocalToWorldMatrix());
}
}  // namespace Isetta
