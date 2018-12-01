/*
 * Copyright (c) 2018 Isetta
 */
#include "EscapeExit.h"

namespace Isetta {
// Register escape key to exit
void EscapeExit::OnEnable() {
  handle = Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                           []() { Application::Exit(); });
}
// Unregister escape key
void EscapeExit::OnDisable() {
  Input::UnregisterKeyPressCallback(KeyCode::ESCAPE, handle);
}
}  // namespace Isetta