/*
 * Copyright (c) 2018 Isetta
 */
#include "Input/Input.h"
#include "Core/Math/Vector2.h"
#include "Input/InputModule.h"

namespace Isetta {

InputModule* Input::inputModule{nullptr};

void Input::RegisterWindowCloseCallback(const std::function<void()>& callback) {
  inputModule->RegisterWindowCloseCallback(callback);
}
bool Input::IsKeyPressed(KeyCode key) { return inputModule->IsKeyPressed(key); }
U64 Input::RegisterKeyPressCallback(KeyCode key,
                                    const std::function<void()>& callback) {
  return inputModule->RegisterKeyPressCallback(key, callback);
}

void Input::UnregisterKeyPressCallback(KeyCode key, U64 handle) {
  inputModule->UnregisterKeyPressCallback(key, handle);
}

U64 Input::RegisterKeyReleaseCallback(KeyCode key,
                                      const std::function<void()>& callback) {
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
                                      const std::function<void()>& callback) {
  return inputModule->RegisterMousePressCallback(mouseButton, callback);
}

void Input::UnregisterMousePressCallback(MouseButtonCode mouseButton,
                                         U64 handle) {
  inputModule->UnregisterMousePressCallback(mouseButton, handle);
}

U64 Input::RegisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                        const std::function<void()>& callback) {
  return inputModule->RegisterMouseReleaseCallback(mouseButton, callback);
}

void Input::UnregisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                           U64 handle) {
  inputModule->UnregisterMouseReleaseCallback(mouseButton, handle);
}
}  // namespace Isetta
