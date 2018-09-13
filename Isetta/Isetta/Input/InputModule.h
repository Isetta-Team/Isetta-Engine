/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <list>
#include <unordered_map>
#include "Core/IModule.h"
#include "Input/InputEnum.h"

namespace Isetta {
class InputModule {
 public:
  void RegisterWindowCloseCallback(std::function<void()> callback);
  bool IsKeyPressed(KeyCode key);
  void RegisterKeyPressCallback(KeyCode key,
                                const std::function<void()>& callback);
  void RegisterKeyReleaseCallback(KeyCode key,
                                  const std::function<void()>& callback);
  // bool IsMouseButtonPressed(MouseButtonCode mouseButton);
  // void RegisterMousePressCallback(
  //    MouseButtonCode mouseButton,const std::function<void()>& callback);
  // void RegisterMouseReleaseCallback(
  //    MouseButtonCode mouseButton,const std::function<void()>& callback);

 private:
  static GLFWwindow* winHandle;

  InputModule() = default;
  ~InputModule() = default;

  void StartUp(GLFWwindow* win);
  void Update();
  void ShutDown();

  int KeyCodeToGLFWKey(KeyCode key);

  static std::list<std::function<void()>> windowCloseCallbacks;
  static void windowCloseListener(GLFWwindow* win);

  static std::unordered_map<int, std::list<std::function<void()>>>
      keyPressCallbacks;
  static std::unordered_map<int, std::list<std::function<void()>>>
      keyReleaseCallbacks;
  static void keyEventListener(GLFWwindow* win, int key, int scancode,
                               int action, int mods);

  friend class ModuleManager;
};
}  // namespace Isetta
