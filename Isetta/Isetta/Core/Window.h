/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include "Core/IModule.h"
#include "Core/ModuleManager.h"
#include <string>

namespace Isetta {
class WindowModule : IModule {
 private:
  WindowModule() = default;
  ~WindowModule() = default;

  void StartUp() final;
  void Update() final;
  void ShutDown() final;

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