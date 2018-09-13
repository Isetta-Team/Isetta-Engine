/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <functional>
#include "Input/InputEnum.h"

namespace Isetta {
class Input {
 public:
  static void RegisterWindowCloseCallback(
      const std::function<void()>& callback);
  static bool IsKeyPressed(KeyCode key);
  static void RegisterKeyPressCallback(KeyCode key,
                                       const std::function<void()>& callback);
  static void RegisterKeyReleaseCallback(KeyCode key,
                                         const std::function<void()>& callback);
  // static bool IsMouseButtonPressed(MouseButtonCode mouseButton);
  // static void RegisterMousePressCallback(MouseButtonCode mouseButton,
  //                                       const std::function<void()>&
  //                                       callback);
  // static void RegisterMouseReleaseCallback(
  //    MouseButtonCode mouseButton, const std::function<void()>& callback);

 private:
  static class InputModule* inputModule;

  friend class InputModule;
};
}  // namespace Isetta
