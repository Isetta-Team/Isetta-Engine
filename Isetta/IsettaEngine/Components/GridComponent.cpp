/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/GridComponent.h"

#include "Core/Debug/DebugDraw.h"

namespace Isetta {
#ifdef _EDITOR
void GridComponent::Update() {
  // draw a grid and axis at origin
  DebugDraw::Grid();
  DebugDraw::Axis();
}
#endif
}  // namespace Isetta
