/*
 * Copyright (c) 2018 Isetta
 */
#include "Input/Input.h"
#include "Input/InputModule.h"
namespace Isetta {

InputModule* Input::inputModule{nullptr};

void Input::RegisterWindowCloseCallback(const std::function<void()>& callback) {
  inputModule->RegisterWindowCloseCallback(callback);
}
bool Input::IsKeyPressed(KeyCode key) { return inputModule->IsKeyPressed(key); }
void Input::RegisterKeyPressCallback(KeyCode key,
                                     const std::function<void()>& callback) {
  inputModule->RegisterKeyPressCallback(key, callback);
}
void Input::RegisterKeyReleaseCallback(KeyCode key,
                                       const std::function<void()>& callback) {
  inputModule->RegisterKeyReleaseCallback(key, callback);
}
}  // namespace Isetta
