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
U16 Input::RegisterWindowSizeCallback(const Action<int, int>& callback) {
  return inputModule->RegisterWindowSizeCallback(callback);
}
void Input::UnegisterWindowSizeCallback(U64& handle) {
  inputModule->UnegisterWindowSizeCallback(handle);
}
bool Input::IsKeyPressed(KeyCode key) { return inputModule->IsKeyPressed(key); }

U64 Input::RegisterKeyPressCallback(KeyCode key, const Action<>& callback) {
  return inputModule->RegisterKeyPressCallback(key, (ModifierKeys)0, callback);
}
U64 Input::RegisterKeyPressCallback(KeyCode key, ModifierKeys mods,
                                    const Action<>& callback) {
  return inputModule->RegisterKeyPressCallback(key, mods, callback);
}

void Input::UnregisterKeyPressCallback(KeyCode key, U64& handle) {
  inputModule->UnregisterKeyPressCallback(key, (ModifierKeys)0, handle);
}
void Input::UnregisterKeyPressCallback(KeyCode key, ModifierKeys mods,
                                       U64& handle) {
  inputModule->UnregisterKeyPressCallback(key, mods, handle);
}

U64 Input::RegisterKeyReleaseCallback(KeyCode key, const Action<>& callback) {
  return inputModule->RegisterKeyReleaseCallback(key, (ModifierKeys)0,
                                                 callback);
}
U64 Input::RegisterKeyReleaseCallback(KeyCode key, ModifierKeys mods,
                                      const Action<>& callback) {
  return inputModule->RegisterKeyReleaseCallback(key, mods, callback);
}

void Input::UnregisterKeyReleaseCallback(KeyCode key, U64& handle) {
  inputModule->UnregisterKeyReleaseCallback(key, (ModifierKeys)0, handle);
}
void Input::UnregisterKeyReleaseCallback(KeyCode key, ModifierKeys mods,
                                         U64& handle) {
  inputModule->UnregisterKeyReleaseCallback(key, mods, handle);
}

Math::Vector2 Input::GetMousePosition() {
  return inputModule->GetMousePosition();
}

bool Input::IsMouseButtonPressed(MouseButton mouseButton) {
  return inputModule->IsMouseButtonPressed(mouseButton);
}

U64 Input::RegisterMousePressCallback(MouseButton mouseButton,
                                      const Action<>& callback) {
  return inputModule->RegisterMousePressCallback(mouseButton, callback);
}

void Input::UnregisterMousePressCallback(MouseButton mouseButton,
                                         U64& handle) {
  inputModule->UnregisterMousePressCallback(mouseButton, handle);
}

U64 Input::RegisterMouseReleaseCallback(MouseButton mouseButton,
                                        const Action<>& callback) {
  return inputModule->RegisterMouseReleaseCallback(mouseButton, callback);
}

void Input::UnregisterMouseReleaseCallback(MouseButton mouseButton,
                                           U64& handle) {
  inputModule->UnregisterMouseReleaseCallback(mouseButton, handle);
}
U16 Input::RegisterScrollCallback(const Action<double, double>& callback) {
  return inputModule->RegisterScrollCallback(callback);
}
void Input::UnregisterScrollCallback(U64& handle) {
  inputModule->UnregisterScrollCallback(handle);
}

float Input::GetGamepadAxis(GamepadAxis axis) {
  return inputModule->GetGamepadAxis(axis);
}

bool Input::IsGamepadButtonPressed(GamepadButton button) {
  return inputModule->IsGamepadButtonPressed(button);
}
void Input::Clear() { inputModule->Clear(); }
}  // namespace Isetta
