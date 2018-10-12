/*
 * Copyright (c) 2018 Isetta
 */
#include "PlayerController.h"
#include "Custom/IsettaCore.h"

namespace Isetta {
void PlayerController::OnEnable() {}

void PlayerController::Update() {
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  if (Input::IsKeyPressed(KeyCode::KP_4)) {
    // GetTransform().RotateLocal(GetTransform().GetUp(), rotSpeed * dt);
    GetTransform().RotateLocal(Math::Vector3{0, rotSpeed * dt, 0});
  }

  if (Input::IsKeyPressed(KeyCode::KP_6)) {
    // GetTransform().RotateLocal(GetTransform().GetUp(), -rotSpeed * dt);
    GetTransform().RotateLocal(Math::Vector3{0, -rotSpeed * dt, 0});
  }

  if (Input::IsKeyPressed(KeyCode::KP_8)) {
    // GetTransform().RotateLocal(GetTransform().GetLeft(), -rotSpeed * dt);
    GetTransform().RotateLocal(Math::Vector3{-rotSpeed * dt, 0, 0});
  }

  if (Input::IsKeyPressed(KeyCode::KP_2)) {
    // GetTransform().RotateLocal(GetTransform().GetLeft(), rotSpeed * dt);
    GetTransform().RotateLocal(Math::Vector3{rotSpeed * dt, 0, 0});
  }

  if (Input::IsKeyPressed(KeyCode::KP_7)) {
    // GetTransform().RotateLocal(GetTransform().GetForward(), -rotSpeed * dt);
    GetTransform().RotateLocal(Math::Vector3{0, 0, -rotSpeed * dt});
  }

  if (Input::IsKeyPressed(KeyCode::KP_9)) {
    // GetTransform().RotateLocal(GetTransform().GetForward(), rotSpeed * dt);
    GetTransform().RotateLocal(Math::Vector3{0, 0, rotSpeed * dt});
  }

  if (Input::IsKeyPressed(KeyCode::KP_5)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * moveSpeed * dt);
  }

  // if (Input::IsKeyPressed(KeyCode::DOWN)) {
    // GetTransform().TranslateWorld(GetTransform().GetForward() * -moveSpeed * dt);
  // }
  // Math::Matrix4 mat = GetTransform().GetLocalToWorldMatrix();
  // DebugDraw::Cube(mat * Math::Matrix4::Translate(Math::Vector3{0, 1.1, 2.8}) *
                      // Math::Matrix4::Scale(2.2 * Math::Vector3::one),
                  // Color::brown);
  DebugDraw::AxisSphere(GetTransform().GetLocalToWorldMatrix());
  if (GetTransform().GetParent() != nullptr) {
    DebugDraw::Line(GetTransform().GetWorldPos(), GetTransform().GetParent()->GetWorldPos());
  }
}

void PlayerController::GuiUpdate() {
#if _DEBUG
  // GetTransform().DrawGUI();
#endif
}
}  // namespace Isetta
