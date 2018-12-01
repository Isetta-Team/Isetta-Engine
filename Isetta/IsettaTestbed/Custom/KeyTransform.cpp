/*
 * Copyright (c) 2018 Isetta
 */
#include "KeyTransform.h"

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
  Input::RegisterKeyPressCallback(KeyCode::PAGE_UP, [&]() {
    if (pressed) return;
    this->transform->TranslateLocal(step * Math::Vector3::up);
  });
  Input::RegisterKeyPressCallback(KeyCode::PAGE_DOWN, [&]() {
    if (pressed) return;
    this->transform->TranslateLocal(step * Math::Vector3::down);
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
  Input::RegisterKeyPressCallback(KeyCode::NUM4, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::up, step);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM5, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::back, step);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM6, [&]() {
    if (pressed) return;
    this->transform->RotateLocal(Math::Vector3::forward, step);
  });

  // Scale
  Input::RegisterKeyPressCallback(KeyCode::NUM7, [&]() {
    if (pressed) return;
    this->transform->SetLocalScale(this->transform->GetLocalScale() * 1.1);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM8, [&]() {
    if (pressed) return;
    this->transform->SetLocalScale(this->transform->GetLocalScale() * .9);
  });

  // Toggle between step and continuous
  Input::RegisterKeyPressCallback(KeyCode::M, [&]() { pressed = !pressed; });
}

void KeyTransform::Update() {
  if (!pressed) return;
  if (Input::IsKeyPressed(KeyCode::RIGHT_ARROW)) {
    this->transform->TranslateLocal(10 * step * Math::Vector3::left *
                                    Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::LEFT_ARROW)) {
    this->transform->TranslateLocal(10 * step * Math::Vector3::right *
                                    Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::DOWN_ARROW)) {
    this->transform->TranslateLocal(10 * step * Math::Vector3::forward *
                                    Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::UP_ARROW)) {
    this->transform->TranslateLocal(10 * step * Math::Vector3::back *
                                    Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::PAGE_UP)) {
    this->transform->TranslateLocal(10 * step * Math::Vector3::up *
                                    Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::PAGE_DOWN)) {
    this->transform->TranslateLocal(10 * step * Math::Vector3::down *
                                    Time::GetDeltaTime());
  }

  // Rotation
  if (Input::IsKeyPressed(KeyCode::NUM1)) {
    this->transform->RotateLocal(Math::Vector3::right,
                                 50 * step * Time::GetDeltaTime());
  };
  if (Input::IsKeyPressed(KeyCode::NUM2)) {
    this->transform->RotateLocal(Math::Vector3::left,
                                 50 * step * Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::NUM3)) {
    this->transform->RotateLocal(Math::Vector3::down,
                                 50 * step * Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::NUM4)) {
    this->transform->RotateLocal(Math::Vector3::up,
                                 50 * step * Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::NUM5)) {
    this->transform->RotateLocal(Math::Vector3::back,
                                 50 * step * Time::GetDeltaTime());
  }
  if (Input::IsKeyPressed(KeyCode::NUM6)) {
    this->transform->RotateLocal(Math::Vector3::forward,
                                 50 * step * Time::GetDeltaTime());
  }

  // Scale
  if (Input::IsKeyPressed(KeyCode::NUM7)) {
    this->transform->SetLocalScale(this->transform->GetLocalScale() *
                                   (1 + .25 * Time::GetDeltaTime()));
  }
  if (Input::IsKeyPressed(KeyCode::NUM8)) {
    this->transform->SetLocalScale(this->transform->GetLocalScale() *
                                   (1 - .25 * Time::GetDeltaTime()));
  }
}
}  // namespace Isetta
