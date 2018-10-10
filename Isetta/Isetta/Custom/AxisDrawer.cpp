/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/AxisDrawer.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Time/Clock.h"
#include "EngineLoop.h"
#include "Scene/Transform.h"

namespace Isetta {

void AxisDrawer::OnEnable() {}

void AxisDrawer::Update() {
  auto& transform = GetTransform();
  float dt = EngineLoop::GetGameClock().GetDeltaTime() * 2;
  DebugDraw::Line(transform.GetWorldPos(),
                  transform.GetWorldPos() + transform.GetForward(), Color::blue,
                  1, dt, false);
  DebugDraw::Line(transform.GetWorldPos(),
                  transform.GetWorldPos() + transform.GetLeft(), Color::red, 1,
                  dt, false);
  DebugDraw::Line(transform.GetWorldPos(),
                  transform.GetWorldPos() + transform.GetUp(), Color::green, 1,
                  dt, false);
}
}  // namespace Isetta
