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
U64 GLFWInput::RegisterWindowSizeCallback(
    const Action<GLFWwindow*, int, int>& callback) {
  return inputModule->RegisterWindowSizeGLFWCallback(callback);
}
void GLFWInput::UnegisterWindowSizeCallback(U64 handle) {
  inputModule->UnegisterWindowSizeGLFWCallback(handle);
}

U64 GLFWInput::RegisterMouseButtonCallback(
    const Action<GLFWwindow*, int, int, int>& callback) {
  return inputModule->RegisterMouseButtonGLFWCallback(callback);
}

void GLFWInput::UnregisterMouseButtonCallback(U64 handle) {
  inputModule->UnregisterMouseButtonGLFWCallback(handle);
}

U64 GLFWInput::RegisterKeyCallback(
    const Action<GLFWwindow*, int, int, int, int>& callback) {
  return inputModule->RegisterKeyGLFWCallback(callback);
}

void GLFWInput::UnegisterKeyCallback(U64 handle) {
  inputModule->UnegisterKeyGLFWCallback(handle);
}
U64 GLFWInput::RegisterScrollCallback(
    const Action<GLFWwindow*, double, double>& callback) {
  return inputModule->RegisterScrollGLFWCallback(callback);
}
void GLFWInput::UnegisterScrollCallback(U64 handle) {
  inputModule->UnregisterScrollGLFWCallback(handle);
}

U64 GLFWInput::RegisterCharCallback(
    const Action<GLFWwindow*, unsigned int>& callback) {
  return inputModule->RegisterCharGLFWCallback(callback);
}

void GLFWInput::UnegisterCharCallback(U64 handle) {
  inputModule->UnegisterCharGLFWCallback(handle);
}

}  // namespace Isetta
