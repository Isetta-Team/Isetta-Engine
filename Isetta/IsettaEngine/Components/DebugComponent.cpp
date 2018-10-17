/*
 * Copyright (c) 2018 Isetta
 */
#include "DebugComponent.h"

#include "Core/Debug/DebugDraw.h"

namespace Isetta {
void DebugComponent::Update() {
  DebugDraw::Grid();
  DebugDraw::Axis();
}
}  // namespace Isetta
