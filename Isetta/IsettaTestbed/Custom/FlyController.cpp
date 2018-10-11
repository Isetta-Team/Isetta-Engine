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
  lastFrameMousePos = Input::GetMousePosition();
}

void FlyController::Update() {
  float speed = 30.f;
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  if (Input::IsKeyPressed(KeyCode::W)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * speed * -1);
  }

  if (Input::IsKeyPressed(KeyCode::S)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * speed * 1);
  }

  if (Input::IsKeyPressed(KeyCode::D)) {
    GetTransform().TranslateWorld(GetTransform().GetLeft() * dt * speed * 1);
  }

  if (Input::IsKeyPressed(KeyCode::A)) {
    GetTransform().TranslateWorld(GetTransform().GetLeft() * dt * speed * -1);
  }

  if (Input::IsKeyPressed(KeyCode::E)) {
    GetTransform().TranslateWorld(GetTransform().GetUp() * dt * speed * 1);
  }

  if (Input::IsKeyPressed(KeyCode::Q)) {
    GetTransform().TranslateWorld(GetTransform().GetUp() * dt * speed * -1);
  }


  // float smooth = -0.1f;
  // float dy = Input::GetMousePosition().y - lastFrameMousePos.y;
  // float dx = Input::GetMousePosition().x - lastFrameMousePos.x;

  // GetTransform().RotateWorld(Math::Vector3{dy, dx, 0.f} * smooth);

  lastFrameMousePos = Input::GetMousePosition();
}

void FlyController::GuiUpdate() {
  GUI::Text(RectTransform{{0, 200, 300, 100}, GUI::Pivot::Top},
            Util::StrFormat("I am %s and I am %d", "Yidi", 10),
            GUI::TextStyle{false, false, Color::white});
  // LOG_INFO(Debug::Channel::General, "Hi I'mYidi");
}
}  // namespace Isetta
