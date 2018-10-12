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
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  if (Input::IsKeyPressed(KeyCode::W)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * flySpeed * -1);
  }

  if (Input::IsKeyPressed(KeyCode::S)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * flySpeed * 1);
  }

  if (Input::IsKeyPressed(KeyCode::D)) {
    GetTransform().TranslateWorld(GetTransform().GetLeft() * dt * flySpeed * 1);
  }

  if (Input::IsKeyPressed(KeyCode::A)) {
    GetTransform().TranslateWorld(GetTransform().GetLeft() * dt * flySpeed * -1);
  }

  if (Input::IsKeyPressed(KeyCode::E)) {
    GetTransform().TranslateWorld(GetTransform().GetUp() * dt * flySpeed * 1);
  }

  if (Input::IsKeyPressed(KeyCode::Q)) {
    GetTransform().TranslateWorld(GetTransform().GetUp() * dt * flySpeed * -1);
  }


  Math::Vector2 mouseDelta = Input::GetMousePosition() - lastFrameMousePos;
  rotX += mouseDelta.x * lookRotationSpeed;
  rotY += mouseDelta.y * lookRotationSpeed;

  GetTransform().SetLocalRot(Math::Quaternion::FromAngleAxis(Math::Vector3::up, rotX));
  GetTransform().RotateLocal(GetTransform().GetLeft(), rotY);

  lastFrameMousePos = Input::GetMousePosition();
}

void FlyController::GuiUpdate() {
}
}  // namespace Isetta
