/*
 * Copyright (c) 2018 Isetta
 */
#include "Input/Input.h"

#include "Core/Math/Vector2.h"
#include "Input/InputModule.h"

namespace Isetta {

InputModule* Input::inputModule{nullptr};

void Input::RegisterWindowCloseCallback(const Action<>& callback) {
  inputModule->RegisterWindowCloseCallback(callback);
}
U16 Input::RegisterWinSizeCallback(const Action<int, int>& callback) {
  return inputModule->RegisterWindowSizeCallback(callback);
}
void Input::UnegisterWindowSizeCallback(U16 handle) {
  inputModule->UnegisterWindowSizeCallback(handle);
}
bool Input::IsKeyPressed(KeyCode key) { return inputModule->IsKeyPressed(key); }
U16 Input::RegisterKeyPressCallback(KeyCode key, const Action<>& callback) {
  return inputModule->RegisterKeyPressCallback(key, callback);
}

void Input::UnregisterKeyPressCallback(KeyCode key, U16 handle) {
  inputModule->UnregisterKeyPressCallback(key, handle);
}

U16 Input::RegisterKeyReleaseCallback(KeyCode key, const Action<>& callback) {
  return inputModule->RegisterKeyReleaseCallback(key, callback);
}

void Input::UnregisterKeyReleaseCallback(KeyCode key, U16 handle) {
  inputModule->UnregisterKeyReleaseCallback(key, handle);
}

Math::Vector2 Input::GetMousePosition() {
  return inputModule->GetMousePosition();
}

bool Input::IsMouseButtonPressed(MouseButtonCode mouseButton) {
  return inputModule->IsMouseButtonPressed(mouseButton);
}

U16 Input::RegisterMousePressCallback(MouseButtonCode mouseButton,
                                      const Action<>& callback) {
  return inputModule->RegisterMousePressCallback(mouseButton, callback);
}

void Input::UnregisterMousePressCallback(MouseButtonCode mouseButton,
                                         U16 handle) {
  inputModule->UnregisterMousePressCallback(mouseButton, handle);
}

U16 Input::RegisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                        const Action<>& callback) {
  return inputModule->RegisterMouseReleaseCallback(mouseButton, callback);
}

void Input::UnregisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                           U16 handle) {
  inputModule->UnregisterMouseReleaseCallback(mouseButton, handle);
}
U16 Input::RegisterScrollCallback(const Action<double, double>& callback) {
  return inputModule->RegisterScrollCallback(callback);
}
void Input::UnregisterScrollCallback(U16 handle) {
  inputModule->UnregisterScrollCallback(handle);
}

/// GLFW Callbacks
U16 Input::RegisterMouseButtonGLFWCallback(
    const Action<GLFWwindow*, int, int, int>& callback) {
  return inputModule->RegisterMouseButtonGLFWCallback(callback);
}

void Input::UnregisterMouseButtonGLFWCallback(U16 handle) {
  inputModule->UnregisterMouseButtonGLFWCallback(handle);
}

U16 Input::RegisterKeyGLFWCallback(
    const Action<GLFWwindow*, int, int, int, int>& callback) {
  return inputModule->RegisterKeyGLFWCallback(callback);
}

void Input::UnegisterKeyGLFWCallback(U16 handle) {
  inputModule->UnegisterKeyGLFWCallback(handle);
}
U16 Input::RegisterScrollGLFWCallback(
    const Action<GLFWwindow*, double, double>& callback) {
  return inputModule->RegisterScrollGLFWCallback(callback);
}
void Input::UnegisterScrollGLFWCallback(U16 handle) {
  inputModule->UnregisterScrollGLFWCallback(handle);
}

U16 Input::RegisterCharGLFWCallback(
    const Action<GLFWwindow*, unsigned int>& callback) {
  return inputModule->RegisterCharGLFWCallback(callback);
}

void Input::UnegisterCharGLFWCallback(U16 handle) {
  inputModule->UnegisterCharGLFWCallback(handle);
}

float Input::GetGamepadAxis(GamepadAxis axis) {
  return inputModule->GetGamepadAxis(axis);
}

bool Input::IsGamepadButtonPressed(GamepadButton button) {
  return inputModule->IsGamepadButtonPressed(button);
}
}  // namespace Isetta
