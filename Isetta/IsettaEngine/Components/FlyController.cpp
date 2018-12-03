/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/FlyController.h"
#include "Core/IsettaCore.h"

namespace Isetta {

FlyController::FlyController(bool enableLook) : enableLook{enableLook} {};

void FlyController::OnEnable() {
  // user for look calculation
  lastFrameMousePos = Input::GetMousePosition();

  // disable/enable look
  Input::RegisterKeyPressCallback(KeyCode::F,
                                  [&]() { enableLook = !enableLook; });

  // a key combo that reset fly controller position
  Input::RegisterKeyPressCallback(KeyCode::KP_0, ModifierKeys::CTRL, [&]() {
    transform->SetWorldPos(Math::Vector3::zero);
  });

  // change fly speed
  Input::RegisterScrollCallback([&](double xOffset, double yOffset) {
    flySpeedMultiplier += yOffset;
    flySpeedMultiplier = Math::Util::Max(flySpeedMultiplier, 1.f);
  });
}

void FlyController::Update() {
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  // modify speed
  if (Input::IsKeyPressed(KeyCode::LEFT_SHIFT)) {
    dt *= flySpeedMultiplier;
  } else if (Input::IsKeyPressed(KeyCode::LEFT_CONTROL)) {
    dt /= flySpeedMultiplier;
  }

  // move forward
  // this is a little weird cause the camera's "looking direction" is it's
  // negative Z
  if (Input::IsKeyPressed(KeyCode::W)) {
    transform->TranslateWorld(-transform->GetForward() * dt * flySpeed);
  }

  // move backward
  if (Input::IsKeyPressed(KeyCode::S)) {
    transform->TranslateWorld(transform->GetForward() * dt * flySpeed);
  }

  // move right
  if (Input::IsKeyPressed(KeyCode::D)) {
    transform->TranslateWorld(transform->GetLeft() * dt * flySpeed);
  }

  // move left
  if (Input::IsKeyPressed(KeyCode::A)) {
    transform->TranslateWorld(-transform->GetLeft() * dt * flySpeed);
  }

  // move up
  if (Input::IsKeyPressed(KeyCode::E)) {
    transform->TranslateWorld(transform->GetUp() * dt * flySpeed);
  }

  // move down
  if (Input::IsKeyPressed(KeyCode::Q)) {
    transform->TranslateWorld(-transform->GetUp() * dt * flySpeed);
  }

  // if look is enabled, rotation based on mouse movement
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
}  // namespace Isetta
