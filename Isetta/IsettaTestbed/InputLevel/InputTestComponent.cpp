/*
 * Copyright (c) 2018 Isetta
 */
#include "InputTestComponent.h"
using namespace Isetta;

void InputTestComponent::OnEnable() {
  // Registering Input Key Pressed - callback occurs on key press
  // param1: key to trigger callback
  // param2: callback function
  //  Register returns handle used to Unregister
  //  It is good practice to unregister your input,
  //  LevelUnload will automatically unregister all input
  handleA = Input::RegisterKeyPressCallback(
      KeyCode::A, []() { LOG_INFO(Debug::Channel::General, "A pressed"); });

  // Register Input Key Release - callback occurs on key release
  handleB = Input::RegisterKeyReleaseCallback(
      KeyCode::A, []() { LOG_INFO(Debug::Channel::General, "A released"); });

  // Register Mouse Button Press - callback occurs on mouse press
  handleC = Input::RegisterMousePressCallback(MouseButton::LEFT, [&]() {
    LOG_INFO(Debug::Channel::General,
             {"Left pressed at: " + Input::GetMousePosition().ToString()});

    // Unregister Mouse Press Callback
    //  param1: key/button handle was registered to
    //  param2: handle received on register
    Input::UnregisterMousePressCallback(MouseButton::LEFT, handleC);
    // Invalidate the handle
    handleC = -1;
  });
}

void InputTestComponent::OnDisable() {
  // Unregister Input
  //  It is good practice to unregister all input,
  //  LevelUnload will automatically unregister all input
  Isetta::Input::UnregisterKeyPressCallback(KeyCode::A, handleA);
  Isetta::Input::UnregisterKeyReleaseCallback(KeyCode::A, handleB);
  if (handleC >= 0)
    Input::UnregisterMousePressCallback(MouseButton::LEFT, handleC);
}

void InputTestComponent::Update() {
  // IsXXXPressed checks each frame is key/button/mouse is pressed
  //  Returns true if pressed
  if (Input::IsGamepadButtonPressed(GamepadButton::X)) {
    LOG_INFO(Debug::Channel::General, "X pressing");
  }
  if (Input::IsKeyPressed(KeyCode::B)) {
    LOG_INFO(Debug::Channel::General, "B pressing");
  }
  if (Input::IsMouseButtonPressed(MouseButton::MIDDLE)) {
    LOG_INFO(Debug::Channel::General, "Middle pressing");
  }
}
