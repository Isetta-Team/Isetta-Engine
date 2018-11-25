/*
 * Copyright (c) 2018 Isetta
 */
#include "KeyTransform.h"

#include "Core/Math/Vector3.h"
#include "Core/Time/Time.h"
#include "Input/Input.h"
#include "Scene/Transform.h"

namespace Isetta {
void KeyTransform::OnEnable() {
  // Translation
  Input::RegisterKeyPressCallback(KeyCode::RIGHT_ARROW, [&]() {
    if (pressed) return;
    this->transform->TranslateLocal(step * Math::Vector3::left);
  });
  Input::RegisterKeyPressCallback(KeyCode::LEFT_ARROW, [&]() {
    if (pressed) return;
    this->transform->TranslateLocal(step * Math::Vector3::right);
  });
  Input::RegisterKeyPressCallback(KeyCode::UP_ARROW, [&]() {
    if (pressed) return;
    this->transform->TranslateLocal(step * Math::Vector3::forward);
  });
  Input::RegisterKeyPressCallback(KeyCode::DOWN_ARROW, [&]() {
    if (pressed) return;
    this->transform->TranslateLocal(step * Math::Vector3::back);
  });

  // Rotation
  Input::RegisterKeyPressCallback(KeyCode::NUM1, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::right, step);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM2, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::left, step);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM3, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::down, step);
  });
  Input::RegisterKeyPressCallback(KeyCode::KP_6, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::up, step);
  });
  Input::RegisterKeyPressCallback(KeyCode::KP_7, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::back, step);
  });
  Input::RegisterKeyPressCallback(KeyCode::KP_9, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::forward, step);
  });

  // Scale
  Input::RegisterKeyPressCallback(KeyCode::KP_1, [&]() {
    if (pressed) return;
    this->transform->SetLocalScale(this->transform->GetLocalScale() *
                                       1.1);
  });
  Input::RegisterKeyPressCallback(KeyCode::KP_3, [&]() {
    if (pressed) return;
    this->transform->SetLocalScale(this->transform->GetLocalScale() *
                                       .9);
  });

  Input::RegisterKeyPressCallback(KeyCode::M, [&]() { pressed = !pressed; });
}
void KeyTransform::Update() {
  if (!pressed) return;
  if (Input::IsKeyPressed(KeyCode::RIGHT_ARROW)) {
    this->transform->TranslateLocal(
        10 * step * Math::Vector3::left *
        Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::LEFT_ARROW)) {
    this->transform->TranslateLocal(
        10 * step * Math::Vector3::right *
        Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::DOWN_ARROW)) {
    this->transform->TranslateLocal(
        10 * step * Math::Vector3::forward *
        Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::UP_ARROW)) {
    this->transform->TranslateLocal(
        10 * step * Math::Vector3::back *
        Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::PAGE_UP)) {
    this->transform->TranslateLocal(
        10 * step * Math::Vector3::up *
        Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::PAGE_DOWN)) {
    this->transform->TranslateLocal(
        10 * step * Math::Vector3::down *
        Time::GetDeltaTime());
  }

  // Rotation
  if (Input::IsKeyPressed(KeyCode::KP_8)) {
    this->transform->RotateLocal(
        Math::Vector3::right,
        50 * step * Time::GetDeltaTime());
  };
  if (Input::IsKeyPressed(KeyCode::KP_2)) {
    this->transform->RotateLocal(
        Math::Vector3::left,
        50 * step * Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::KP_4)) {
    this->transform->RotateLocal(
        Math::Vector3::down,
        50 * step * Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::KP_6)) {
    this->transform->RotateLocal(
        Math::Vector3::up,
        50 * step * Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::KP_7)) {
    this->transform->RotateLocal(
        Math::Vector3::back,
        50 * step * Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::KP_9)) {
    this->transform->RotateLocal(
        Math::Vector3::forward,
        50 * step * Time::GetDeltaTime());
  }

  // Scale
  if (Input::IsKeyPressed(KeyCode::KP_1)) {
    this->transform->SetLocalScale(
        this->transform->GetLocalScale() *
        (1 + .25 * Time::GetDeltaTime()));
  }
  if (Input::IsKeyPressed(KeyCode::KP_3)) {
    this->transform->SetLocalScale(
        this->transform->GetLocalScale() *
        (1 - .25 * Time::GetDeltaTime()));
  }

  // Rotation
  if (Input::IsKeyPressed(KeyCode::KP_8)) {
    this->transform->RotateLocal(
        Math::Vector3::right,
        50 * step * EngineLoop::GetGameClock().GetDeltaTime());
  };
  if (Input::IsKeyPressed(KeyCode::KP_2)) {
    this->transform->RotateLocal(
        Math::Vector3::left,
        50 * step * EngineLoop::GetGameClock().GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::KP_4)) {
    this->transform->RotateLocal(
        Math::Vector3::down,
        50 * step * EngineLoop::GetGameClock().GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::KP_6)) {
    this->transform->RotateLocal(
        Math::Vector3::up,
        50 * step * EngineLoop::GetGameClock().GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::KP_7)) {
    this->transform->RotateLocal(
        Math::Vector3::back,
        50 * step * EngineLoop::GetGameClock().GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::KP_9)) {
    this->transform->RotateLocal(
        Math::Vector3::forward,
        50 * step * EngineLoop::GetGameClock().GetDeltaTime());
  }

  // Scale
  if (Input::IsKeyPressed(KeyCode::KP_1)) {
    this->transform->SetLocalScale(
        this->transform->GetLocalScale() *
        (1 + .25 * EngineLoop::GetGameClock().GetDeltaTime()));
  }
  if (Input::IsKeyPressed(KeyCode::KP_3)) {
    this->transform->SetLocalScale(
        this->transform->GetLocalScale() *
        (1 - .25 * EngineLoop::GetGameClock().GetDeltaTime()));
  }
}
}  // namespace Isetta
