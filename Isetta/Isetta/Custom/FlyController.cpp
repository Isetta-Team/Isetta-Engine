/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/FlyController.h"
#include "Core/Debug/DebugDraw.h"
#include "Graphics/GUI.h"
#include "Util.h"
#include "Graphics/RectTransform.h"
#include "Input/Input.h"
#include "Scene/Transform.h"
#include "EngineLoop.h"
#include "Core/Time/Clock.h"

namespace Isetta {

void FlyController::OnEnable() {
  SetAttribute(ComponentAttributes::NEED_UPDATE, true);
}

void FlyController::Update() {
  float speed = 100.f;
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  if (Input::IsKeyPressed(KeyCode::W)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * speed * -1);
  }
}

void FlyController::GuiUpdate() {
  GUI::Text(RectTransform{{0, 200, 300, 100}, GUI::Pivot::Top},
            Util::StrFormat("I am %s and I am %d", "Yidi", 10),
            GUI::TextStyle{false, false, Color::white});
  // LOG_INFO(Debug::Channel::General, "Hi I'mYidi");
}
}  // namespace Isetta
