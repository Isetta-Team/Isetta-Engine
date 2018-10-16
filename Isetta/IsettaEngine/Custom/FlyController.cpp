/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/FlyController.h"
#include "Custom/IsettaCore.h"

namespace Isetta {

void FlyController::OnEnable() {
  SetAttribute(ComponentAttributes::NEED_UPDATE, true);
  lastFrameMousePos = Input::GetMousePosition();
  Input::RegisterKeyPressCallback(KeyCode::F,
                                  [&]() { enableLook = !enableLook; });
}

void FlyController::Update() {
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  if (Input::IsKeyPressed(KeyCode::W)) {
    GetTransform().TranslateWorld(-GetTransform().GetForward() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::S)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::D)) {
    GetTransform().TranslateWorld(GetTransform().GetLeft() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::A)) {
    GetTransform().TranslateWorld(-GetTransform().GetLeft() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::E)) {
    GetTransform().TranslateWorld(GetTransform().GetUp() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::Q)) {
    GetTransform().TranslateWorld(-GetTransform().GetUp() * dt * flySpeed);
  }

  if (enableLook) {
    Math::Vector2 mouseDelta = Input::GetMousePosition() - lastFrameMousePos;
    rotX += mouseDelta.x * lookRotationSpeed;
    rotY += mouseDelta.y * lookRotationSpeed;

    GetTransform().SetLocalRot(
        Math::Quaternion::FromAngleAxis(Math::Vector3::up, rotX));
    GetTransform().RotateLocal(GetTransform().GetLeft(), rotY);
  }
  lastFrameMousePos = Input::GetMousePosition();
}

void FlyController::GuiUpdate() {}
}  // namespace Isetta
