/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/AxisDrawer.h"
#include "Core/Debug/DebugDraw.h"
#include "Scene/Transform.h"

namespace Isetta {
#ifdef _EDITOR
void AxisDrawer::Update() {
  // draw the x-y-z axes at the entity's worldPos
  DebugDraw::Axis(transform->GetLocalToWorldMatrix());
}
#endif
}  // namespace Isetta
