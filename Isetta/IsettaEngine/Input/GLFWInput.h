/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector2.h"
#include "Input/KeyCode.h"

namespace Isetta {
class GLFWInput {
 public:
  static void RegisterWindowCloseCallback(const Action<GLFWwindow*>& callback);
  static U16 RegisterWindowSizeCallback(
      const Action<GLFWwindow*, int, int>& callback);
  static void UnegisterWindowSizeCallback(U16 handle);
  static U16 RegisterMouseButtonCallback(
      const Action<GLFWwindow*, int, int, int>& callback);
  static void UnregisterMouseButtonCallback(U16 handle);
  static U16 RegisterKeyCallback(
      const Action<GLFWwindow*, int, int, int, int>& callback);
  static void UnegisterKeyCallback(U16 handle);
  static U16 RegisterScrollCallback(
      const Action<GLFWwindow*, double, double>& callback);
  static void UnegisterScrollCallback(U16 handle);
  static U16 RegisterCharCallback(
      const Action<GLFWwindow*, unsigned int>& callback);
  static void UnegisterCharCallback(U16 handle);

 private:
  static class InputModule* inputModule;

  friend class InputModule;
};
}  // namespace Isetta
