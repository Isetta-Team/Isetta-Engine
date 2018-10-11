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
U64 Input::RegisterWinSizeCallback(const Action<int, int>& callback) {
  return inputModule->RegisterWindowSizeCallback(callback);
}
void Input::UnegisterWindowSizeCallback(U64 handle) {
  inputModule->UnegisterWindowSizeCallback(handle);
}
bool Input::IsKeyPressed(KeyCode key) { return inputModule->IsKeyPressed(key); }
U64 Input::RegisterKeyPressCallback(KeyCode key, const Action<>& callback) {
  return inputModule->RegisterKeyPressCallback(key, callback);
}

void Input::UnregisterKeyPressCallback(KeyCode key, U64 handle) {
  inputModule->UnregisterKeyPressCallback(key, handle);
}

U64 Input::RegisterKeyReleaseCallback(KeyCode key, const Action<>& callback) {
  return inputModule->RegisterKeyReleaseCallback(key, callback);
}

void Input::UnregisterKeyReleaseCallback(KeyCode key, U64 handle) {
  inputModule->UnregisterKeyReleaseCallback(key, handle);
}

Math::Vector2 Input::GetMousePosition() {
  return inputModule->GetMousePosition();
}

bool Input::IsMouseButtonPressed(MouseButtonCode mouseButton) {
  return inputModule->IsMouseButtonPressed(mouseButton);
}

U64 Input::RegisterMousePressCallback(MouseButtonCode mouseButton,
                                      const Action<>& callback) {
  return inputModule->RegisterMousePressCallback(mouseButton, callback);
}

void Input::UnregisterMousePressCallback(MouseButtonCode mouseButton,
                                         U64 handle) {
  inputModule->UnregisterMousePressCallback(mouseButton, handle);
}

U64 Input::RegisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                        const Action<>& callback) {
  return inputModule->RegisterMouseReleaseCallback(mouseButton, callback);
}

void Input::UnregisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                           U64 handle) {
  inputModule->UnregisterMouseReleaseCallback(mouseButton, handle);
}

U64 Input::RegisterMouseButtonCallback(
    const Action<GLFWwindow*, int, int, int>& callback) {
  return inputModule->RegisterMouseButtonCallback(callback);
}

void Input::UnregisterMouseButtonCallback(U64 handle) {
  inputModule->UnregisterMouseButtonCallback(handle);
}

U64 Input::RegisterKeyCallback(
    const Action<GLFWwindow*, int, int, int, int>& callback) {
  return inputModule->RegisterKeyCallback(callback);
}

void Input::UnegisterKeyCallback(U64 handle) {
  inputModule->UnegisterKeyCallback(handle);
}
U64 Input::RegisterScrollCallback(
    const Action<GLFWwindow*, double, double>& callback) {
  return inputModule->RegisterScrollCallback(callback);
}

void Input::UnegisterScrollCallback(U64 handle) {
  inputModule->UnegisterScrollCallback(handle);
}

U64 Input::RegisterCharCallback(
    const Action<GLFWwindow*, unsigned int>& callback) {
  return inputModule->RegisterCharCallback(callback);
}

void Input::UnegisterCharCallback(U64 handle) {
  inputModule->UnegisterCharCallback(handle);
}
}  // namespace Isetta
