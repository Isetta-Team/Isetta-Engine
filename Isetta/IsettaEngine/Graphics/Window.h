/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include "Core/Config/CVar.h"

namespace Isetta::Math {
class Vector2;
}

namespace Isetta {
class WindowModule {
 public:
  struct WindowConfig {
    CVar<int> windowWidth{"window_width", 1024};
    CVar<int> windowHeight{"window_height", 768};
    CVarString windowTitle{"window_title", "Game"};
    CVar<int> windowFullScreen{"window_fullscreen", 0};
    CVar<int> windowShowCursor{"window_show_cursor", 1};
  };

 private:
  WindowModule() = default;
  ~WindowModule() = default;

  void StartUp();
  void Update(float deltaTime);
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
  friend class EngineLoop;
};
}  // namespace Isetta
