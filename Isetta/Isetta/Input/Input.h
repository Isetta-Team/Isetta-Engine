/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <functional>
#include "Core/IsettaAlias.h"
#include "Input/InputEnum.h"
#include "Core/Math/Vector2.h"

namespace Isetta {
class Input {
 public:
  static void RegisterWindowCloseCallback(
      const std::function<void()>& callback);
  static bool IsKeyPressed(KeyCode key);
  static U64 RegisterKeyPressCallback(KeyCode key,
                                      const std::function<void()>& callback);
  static void UnregisterKeyPressCallback(KeyCode key, U64 handle);
  static U64 RegisterKeyReleaseCallback(KeyCode key,
                                        const std::function<void()>& callback);
  static void UnregisterKeyReleaseCallback(KeyCode key, U64 handle);

  static Math::Vector2 GetMousePosition();
  static bool IsMouseButtonPressed(MouseButtonCode mouseButton);
  static U64 RegisterMousePressCallback(MouseButtonCode mouseButton,
                                        const std::function<void()>& callback);
  static void UnregisterMousePressCallback(MouseButtonCode mouseButton,
                                           U64 handle);
  static U64 RegisterMouseReleaseCallback(
      MouseButtonCode mouseButton, const std::function<void()>& callback);
  static void UnregisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                             U64 handle);

 private:
  static class InputModule* inputModule;

  friend class InputModule;
};
}  // namespace Isetta
