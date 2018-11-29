/*
 * Copyright (c) 2018 Isetta
 */
#include "EscapeExit.h"

#include "Application.h"
#include "Input/Input.h"
#include "Input/KeyCode.h"

namespace Isetta {
void EscapeExit::OnEnable() {
  handle = Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                           []() { Application::Exit(); });
}
void EscapeExit::OnDisable() {
  Input::UnregisterKeyPressCallback(KeyCode::ESCAPE, handle);
}
}  // namespace Isetta