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

  DebugDraw::Axis(GetTransform().GetLocalToWorldMatrix(), Color::red,
                  Color::green, Color::blue, 2, 0, 2, false);
}
}  // namespace Isetta
