/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/FlyController.h"
#include "Custom/IsettaCore.h"

namespace Isetta {

FlyController::FlyController(bool inControl) : enableLook{inControl} {};

void FlyController::OnEnable() {
  SetAttribute(ComponentAttributes::NEED_UPDATE, true);
  lastFrameMousePos = Input::GetMousePosition();
  Input::RegisterKeyPressCallback(KeyCode::F,
                                  [&]() { enableLook = !enableLook; });
  Input::RegisterKeyPressCallback(KeyCode::KP_0, ModifierKeys::CTRL, [&]() {
    transform->SetWorldPos(Math::Vector3::zero);
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
    transform->TranslateWorld(-transform->GetForward() * dt *
                                   flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::S)) {
    transform->TranslateWorld(transform->GetForward() * dt *
                                   flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::D)) {
    transform->TranslateWorld(transform->GetLeft() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::A)) {
    transform->TranslateWorld(-transform->GetLeft() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::E)) {
    transform->TranslateWorld(transform->GetUp() * dt * flySpeed);
  }

  if (Input::IsKeyPressed(KeyCode::Q)) {
    transform->TranslateWorld(-transform->GetUp() * dt * flySpeed);
  }

  if (enableLook) {
    Math::Vector2 mouseDelta = Input::GetMousePosition() - lastFrameMousePos;
    rotX += mouseDelta.x * lookRotationSpeed;
    rotY += mouseDelta.y * lookRotationSpeed;

    transform->SetLocalRot(
        Math::Quaternion::FromAngleAxis(Math::Vector3::up, rotX));
    transform->RotateLocal(transform->GetLeft(), rotY);
  }
  lastFrameMousePos = Input::GetMousePosition();
}

void FlyController::GuiUpdate() {}
}  // namespace Isetta
