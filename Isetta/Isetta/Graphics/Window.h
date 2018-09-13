/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include "Core/IModule.h"
#include "Core/ModuleManager.h"

namespace Isetta {
class WindowModule {
 private:
  WindowModule() = default;
  ~WindowModule() = default;

  void StartUp();
  void Update();
  void ShutDown();

  std::string winTitle;
  int initWinWidth;
  int initWinHeight;
  bool winFullScreen;
  bool winShowCursor;

  void InitWindow();
  void InitRenderConfig();

  GLFWwindow* winHandle;

  class RenderModule* renderModule;
  class InputModule* inputModule;
  friend class ModuleManager;
};
}  // namespace Isetta
