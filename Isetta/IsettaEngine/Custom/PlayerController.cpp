/*
 * Copyright (c) 2018 Isetta
 */
#include "PlayerController.h"
#include "Custom/IsettaCore.h"

namespace Isetta {
void PlayerController::OnEnable() {}

void PlayerController::Update() {
  float dt = EngineLoop::GetGameClock().GetDeltaTime();
  LOG_INFO(Debug::Channel::General, "Speed %f, %f", rotSpeed, moveSpeed);

  if (Input::IsKeyPressed(KeyCode::KP_4)) {
    GetTransform().RotateLocal(GetTransform().GetUp(), rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::KP_6)) {
    GetTransform().RotateLocal(GetTransform().GetUp(), -rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::KP_8)) {
    GetTransform().RotateLocal(GetTransform().GetLeft(), -rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::KP_2)) {
    GetTransform().RotateLocal(GetTransform().GetLeft(), rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::KP_7)) {
    GetTransform().RotateLocal(GetTransform().GetForward(), -rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::KP_9)) {
    GetTransform().RotateLocal(GetTransform().GetForward(), rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::UP)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * moveSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::DOWN)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * -moveSpeed * dt);
  }
}

void PlayerController::GuiUpdate() {
#if _DEBUG
  GetTransform().DrawGUI();
#endif
}
}  // namespace Isetta
