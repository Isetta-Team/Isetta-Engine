/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/IsettaAlias.h"
#include "GLFW/include/GLFW/glfw3.h"

namespace Isetta {
class GLFWInput {
 public:
  static void RegisterWindowCloseCallback(const Action<GLFWwindow*>& callback);
  static U64 RegisterWindowSizeCallback(
      const Action<GLFWwindow*, int, int>& callback);
  static void UnegisterWindowSizeCallback(U64 handle);
  static U64 RegisterMouseButtonCallback(
      const Action<GLFWwindow*, int, int, int>& callback);
  static void UnregisterMouseButtonCallback(U64 handle);
  static U64 RegisterKeyCallback(
      const Action<GLFWwindow*, int, int, int, int>& callback);
  static void UnegisterKeyCallback(U64 handle);
  static U64 RegisterScrollCallback(
      const Action<GLFWwindow*, double, double>& callback);
  static void UnegisterScrollCallback(U64 handle);
  static U64 RegisterCharCallback(
      const Action<GLFWwindow*, unsigned int>& callback);
  static void UnegisterCharCallback(U64 handle);

 private:
  static class InputModule* inputModule;

  friend class InputModule;
};
}  // namespace Isetta
