/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/FlyController.h"
#include "Custom/IsettaCore.h"

namespace Isetta {

void FlyController::OnEnable() {
  SetAttribute(ComponentAttributes::NEED_UPDATE, true);
  lastFrameMousePos = Input::GetMousePosition();
  Input::RegisterKeyPressCallback(KeyCode::F,
                                  [&]() { enableLook = !enableLook; });
  Input::RegisterKeyPressCallback(KeyCode::KP_0, ModifierKeys::CTRL, [&]() {
    GetTransform()->SetWorldPos(Math::Vector3::zero);
  });
  Input::RegisterScrollCallback([&](double xOffset, double yOffset) {
    flyMultiplier += yOffset;
    flyMultiplier = Math::Util::Max(flyMultiplier, 1.f);
  });
}

void FlyController::Update() {
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  if (Input::IsKeyPressed(KeyCode::LEFT_SHIFT)) {
    dt *= flyMultiplier;
  } else if (Input::IsKeyPressed(KeyCode::LEFT_CONTROL)) {
    dt /= flyMultiplier;
  }

  if (Input::IsKeyPressed(KeyCode::W)) {
    GetTransform()->TranslateWorld(-GetTransform()->GetForward() * dt *
                                   flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::S)) {
    GetTransform()->TranslateWorld(GetTransform()->GetForward() * dt *
                                   flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::D)) {
    GetTransform()->TranslateWorld(GetTransform()->GetLeft() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::A)) {
    GetTransform()->TranslateWorld(-GetTransform()->GetLeft() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::E)) {
    GetTransform()->TranslateWorld(GetTransform()->GetUp() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::Q)) {
    GetTransform()->TranslateWorld(-GetTransform()->GetUp() * dt * flySpeed);
  }

  if (enableLook) {
    Math::Vector2 mouseDelta = Input::GetMousePosition() - lastFrameMousePos;
    rotX += mouseDelta.x * lookRotationSpeed;
    rotY += mouseDelta.y * lookRotationSpeed;

    GetTransform()->SetLocalRot(
        Math::Quaternion::FromAngleAxis(Math::Vector3::up, rotX));
    GetTransform()->RotateLocal(GetTransform()->GetLeft(), rotY);
  }
  lastFrameMousePos = Input::GetMousePosition();
}

void FlyController::GuiUpdate() {}
}  // namespace Isetta
