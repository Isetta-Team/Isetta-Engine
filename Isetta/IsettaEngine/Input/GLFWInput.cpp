/*
 * Copyright (c) 2018 Isetta
 */
#include "Input/GLFWInput.h"

#include "Core/Math/Vector2.h"
#include "Input/InputModule.h"

namespace Isetta {

InputModule* GLFWInput::inputModule{nullptr};

void GLFWInput::RegisterWindowCloseCallback(
    const Action<GLFWwindow*>& callback) {
  inputModule->RegisterWindowCloseGLFWCallback(callback);
}
U16 GLFWInput::RegisterWindowSizeCallback(
    const Action<GLFWwindow*, int, int>& callback) {
  return inputModule->RegisterWindowSizeGLFWCallback(callback);
}
void GLFWInput::UnegisterWindowSizeCallback(U16 handle) {
  inputModule->UnegisterWindowSizeGLFWCallback(handle);
}

U16 GLFWInput::RegisterMouseButtonCallback(
    const Action<GLFWwindow*, int, int, int>& callback) {
  return inputModule->RegisterMouseButtonGLFWCallback(callback);
}

void GLFWInput::UnregisterMouseButtonCallback(U16 handle) {
  inputModule->UnregisterMouseButtonGLFWCallback(handle);
}

U16 GLFWInput::RegisterKeyCallback(
    const Action<GLFWwindow*, int, int, int, int>& callback) {
  return inputModule->RegisterKeyGLFWCallback(callback);
}

void GLFWInput::UnegisterKeyCallback(U16 handle) {
  inputModule->UnegisterKeyGLFWCallback(handle);
}
U16 GLFWInput::RegisterScrollCallback(
    const Action<GLFWwindow*, double, double>& callback) {
  return inputModule->RegisterScrollGLFWCallback(callback);
}
void GLFWInput::UnegisterScrollCallback(U16 handle) {
  inputModule->UnregisterScrollGLFWCallback(handle);
}

U16 GLFWInput::RegisterCharCallback(
    const Action<GLFWwindow*, unsigned int>& callback) {
  return inputModule->RegisterCharGLFWCallback(callback);
}

void GLFWInput::UnegisterCharCallback(U16 handle) {
  inputModule->UnegisterCharGLFWCallback(handle);
}

}  // namespace Isetta
