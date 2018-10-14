/*
 * Copyright (c) 2018 Isetta
 */
#include "KeyTransform.h"

#include "Core/Math/Vector3.h"
#include "Input/Input.h"
#include "Scene/Transform.h"

namespace Isetta {
void KeyTransform::OnEnable() {
  Input::RegisterKeyPressCallback(KeyCode::RIGHT_ARROW, [&]() {
    this->GetTransform().TranslateLocal(step * Math::Vector3::left);
  });
  Input::RegisterKeyPressCallback(KeyCode::LEFT_ARROW, [&]() {
    this->GetTransform().TranslateLocal(step * Math::Vector3::right);
  });
  Input::RegisterKeyPressCallback(KeyCode::UP_ARROW, [&]() {
    this->GetTransform().TranslateLocal(step * Math::Vector3::forward);
  });
  Input::RegisterKeyPressCallback(KeyCode::DOWN_ARROW, [&]() {
    this->GetTransform().TranslateLocal(step * Math::Vector3::back);
  });
}
}  // namespace Isetta
