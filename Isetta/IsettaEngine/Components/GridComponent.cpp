/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/GridComponent.h"

#include "Core/Debug/DebugDraw.h"

namespace Isetta {
void GridComponent::Update() {
  DebugDraw::Grid();
  DebugDraw::Axis();
}
}  // namespace Isetta
