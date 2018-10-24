/*
 * Copyright (c) 2018 Isetta
 */
#include "KeyTransform.h"

#include "Core/Math/Vector3.h"
#include "Core/Time/Clock.h"
#include "EngineLoop.h"
#include "Input/Input.h"
#include "Scene/Transform.h"

namespace Isetta {
void KeyTransform::OnEnable() {
  Input::RegisterKeyPressCallback(KeyCode::RIGHT_ARROW, [&]() {
    if (pressed) return;
    this->GetTransform().TranslateLocal(step * Math::Vector3::left);
  });
  Input::RegisterKeyPressCallback(KeyCode::LEFT_ARROW, [&]() {
    if (pressed) return;
    this->GetTransform().TranslateLocal(step * Math::Vector3::right);
  });
  Input::RegisterKeyPressCallback(KeyCode::UP_ARROW, [&]() {
    if (pressed) return;
    this->GetTransform().TranslateLocal(step * Math::Vector3::forward);
  });
  Input::RegisterKeyPressCallback(KeyCode::DOWN_ARROW, [&]() {
    if (pressed) return;
    this->GetTransform().TranslateLocal(step * Math::Vector3::back);
  });

  Input::RegisterKeyPressCallback(KeyCode::M, [&]() { pressed = !pressed; });
}
void KeyTransform::Update() {
  if (!pressed) return;
  if (Input::IsKeyPressed(KeyCode::RIGHT_ARROW)) {
    this->GetTransform().TranslateLocal(
        10 * step * Math::Vector3::left *
        EngineLoop::GetGameClock().GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::LEFT_ARROW)) {
    this->GetTransform().TranslateLocal(
        10 * step * Math::Vector3::right *
        EngineLoop::GetGameClock().GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::UP_ARROW)) {
    this->GetTransform().TranslateLocal(
        10 * step * Math::Vector3::forward *
        EngineLoop::GetGameClock().GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::DOWN_ARROW)) {
    this->GetTransform().TranslateLocal(
        10 * step * Math::Vector3::back *
        EngineLoop::GetGameClock().GetDeltaTime());
  }
}
}  // namespace Isetta
