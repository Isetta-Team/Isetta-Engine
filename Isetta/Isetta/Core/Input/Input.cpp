/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Input/Input.h"
#include "Core/Graphics/Render.h"

namespace Isetta {
std::list<std::function<void()>> InputModule::windowCloseCallbacks{};

void InputModule::RegisterWindowCloseCallback(std::function<void()> callback) {
  windowCloseCallbacks.push_back(callback);
}
bool InputModule::IsKeyPressed(int key) {
  return glfwGetKey(RenderModule::winHandle, key) == GLFW_PRESS;
}

void InputModule::StartUp() {
  glfwSetWindowCloseCallback(RenderModule::winHandle, windowCloseListener);
}

void InputModule::ShutDown() {}

void InputModule::windowCloseListener(GLFWwindow* win) {
  for (const auto& callback : windowCloseCallbacks) {
    callback();
  }
}
}  // namespace Isetta