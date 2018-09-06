/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <functional>
#include <list>
#include <GLFW/glfw3.h>
#include "Core/IModule.h"

namespace Isetta {
class InputModule : IModule {
 public:
  void RegisterWindowCloseCallback(std::function<void()> callback);
  static bool IsKeyPressed(int key);

 private:
  InputModule() = default;
  ~InputModule() final = default;

  void StartUp() final;
  void ShutDown() final;

  // #TODO Solve function pointer namespace problem
  static std::list<std::function<void()>> windowCloseCallbacks;

  static void windowCloseListener(GLFWwindow* win);

  friend class ModuleManager;
};
}  // namespace Isetta