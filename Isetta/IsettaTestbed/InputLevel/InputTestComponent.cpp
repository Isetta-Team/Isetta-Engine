/*
 * Copyright (c) 2018 Isetta
 */
#include "InputTestComponent.h"

#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Input/Input.h"

void InputTestComponent::Start() {
  Isetta::U64 handleA, handleB, handleC;
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::A, [&handleA]() {
    LOG_INFO(Isetta::Debug::Channel::General, "A pressed");
  });
  Isetta::Input::RegisterKeyReleaseCallback(Isetta::KeyCode::A, [&handleB]() {
    LOG_INFO(Isetta::Debug::Channel::General, "A released");
  });
  handleC = Isetta::Input::RegisterMousePressCallback(
      Isetta::MouseButtonCode::MOUSE_LEFT, [&handleC]() {
        LOG_INFO(Isetta::Debug::Channel::General,
                 {"Left pressed at: " +
                  Isetta::Input::GetMousePosition().ToString()});
        Isetta::Input::UnregisterMousePressCallback(
            Isetta::MouseButtonCode::MOUSE_LEFT, handleC);
      });
}

void InputTestComponent::Update() {
  if (Isetta::Input::IsGamepadButtonPressed(Isetta::GamepadButton::X)) {
    LOG_INFO(Isetta::Debug::Channel::General, "X pressing");
  }
  if (Isetta::Input::IsKeyPressed(Isetta::KeyCode::B)) {
    LOG_INFO(Isetta::Debug::Channel::General, "B pressing");
  }
  if (Isetta::Input::IsMouseButtonPressed(
          Isetta::MouseButtonCode::MOUSE_MIDDLE)) {
    LOG_INFO(Isetta::Debug::Channel::General, "Middle pressing");
  }
}
