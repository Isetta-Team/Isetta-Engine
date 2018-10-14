/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/PlayerController.h"
#include "Core/Time/Clock.h"
#include "EngineLoop.h"
#include "Input/Input.h"
#include "Scene/Transform.h"

namespace Isetta {
void PlayerController::Update() {
  float speed = 10.f;
  float dt = EngineLoop::GetGameClock().GetDeltaTime();
  if (Input::IsKeyPressed(KeyCode::LEFT_ARROW)) {
    GetTransform().TranslateWorld(GetTransform().GetLeft() * 1 * speed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::RIGHT_ARROW)) {
    GetTransform().TranslateWorld(GetTransform().GetLeft() * -1 * speed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::P)) {
    GetTransform().Print();
  }
}
}  // namespace Isetta
