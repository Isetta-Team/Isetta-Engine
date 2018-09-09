/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <list>
#include <unordered_map>
#include "Core/IModule.h"
#include "Core/Input/InputEnum.h"

namespace Isetta {
class InputModule : IModule {
 public:
  void RegisterWindowCloseCallback(std::function<void()> callback);
  bool IsKeyPressed(KeyCode key);
  void RegisterKeyPressCallback(KeyCode key,const std::function<void()>& callback);
  void RegisterKeyReleaseCallback(KeyCode key,const std::function<void()>& callback);
  bool IsMouseButtonPressed(MouseButtonCode mouseButton);
  void RegisterMousePressCallback(
      MouseButtonCode mouseButton,const std::function<void()>& callback);
  void RegisterMouseReleaseCallback(
      MouseButtonCode mouseButton,const std::function<void()>& callback);

 private:
  static GLFWwindow* winHandle;

  InputModule(GLFWwindow* winHandle);
  ~InputModule() final = default;

  void StartUp() final;
  void Update() final;
  void ShutDown() final;

  int KeyCodeToGLFWKey(KeyCode key);

  static std::list<std::function<void()>> windowCloseCallbacks;
  static void windowCloseListener(GLFWwindow* win);

  static std::unordered_map<int, std::list<std::function<void()>>>
      keyPressCallbacks;
  static std::unordered_map<int, std::list<std::function<void()>>>
      keyReleaseCallbacks;
  static void keyEventListener(GLFWwindow* win, int key, int scancode,
                               int action, int mods);


  friend class WindowModule;
};
}  // namespace Isetta