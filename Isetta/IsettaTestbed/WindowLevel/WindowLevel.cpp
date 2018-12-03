/*
 * Copyright (c) 2018 Isetta
 */
#include "WindowLevel.h"

#include "Core/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Graphics/Texture.h"
#include "Graphics/Window.h"
#include "Input/Input.h"

using namespace Isetta;

void WindowLevel::Load() {
  Entity* cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  Texture icon{"Images\\isetta_logo.png", true};
  Window::SetIcon(icon);
  icon.Unload();

  Input::RegisterKeyPressCallback(
      KeyCode::F, []() { Window::SetFullscreen(!Window::IsFullscreen()); });

  Input::RegisterKeyPressCallback(
      KeyCode::NUM1, []() { Window::SetCursorIcon(Window::Cursor::Arrow); });
  Input::RegisterKeyPressCallback(
      KeyCode::NUM2, []() { Window::SetCursorIcon(Window::Cursor::IBeam); });
  Input::RegisterKeyPressCallback(
      KeyCode::NUM3, []() { Window::SetCursorIcon(Window::Cursor::VResize); });
  Input::RegisterKeyPressCallback(
      KeyCode::NUM4, []() { Window::SetCursorIcon(Window::Cursor::HResize); });
  Input::RegisterKeyPressCallback(
      KeyCode::NUM5, []() { Window::SetCursorIcon(Window::Cursor::Hand); });
  Input::RegisterKeyPressCallback(KeyCode::NUM6, []() {
    Window::SetCursorIcon(Window::Cursor::Crosshair);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM7, []() {
    Texture icon{"Images\\isetta_logo.png", true};
    Window::SetCursorIcon(icon);
    icon.Unload();
  });

  static Window::CursorMode mode = Window::CursorMode::Normal;
  Input::RegisterKeyPressCallback(KeyCode::SPACE, []() {
    switch (mode) {
      case Window::CursorMode::Normal:
        mode = Window::CursorMode::Hidden;
        break;
      case Window::CursorMode::Hidden:
        mode = Window::CursorMode::Disabled;
        break;
      case Window::CursorMode::Disabled:
        mode = Window::CursorMode::Normal;
        break;
    }
    Window::SetCursorMode(mode);
  });
}
