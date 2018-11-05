/*
 * Copyright (c) 2018 Isetta
 */
#include "Editor.h"

#include "Graphics/GUI.h"
#include "Input/Input.h"
#include "Input/KeyCode.h"
#include "Scene/Entity.h"

#include "Components/Editor/Console.h"
#include "Components/Editor/Hierarchy.h"
#include "Components/Editor/Inspector.h"

namespace Isetta {
Editor::Editor(bool isOpen) {
  console = new Console("Console", isOpen);
  inspector = new Inspector("Inspector", false);
  hierarchy = new Hierarchy("Hierarchy", isOpen, inspector);
}

void Editor::OnEnable() {
  console->OnEnable();
  inspector->OnEnable();
  hierarchy->OnEnable();

  menuHandle = Input::RegisterKeyPressCallback(
      KeyCode::M, ModifierKeys::CTRL | ModifierKeys::SHIFT,
      [&]() { menuOpen = !menuOpen; });
  consoleHandle = Input::RegisterKeyPressCallback(
      KeyCode::C, ModifierKeys::CTRL | ModifierKeys::SHIFT,
      [&]() { console->Open(); });
  inspectorHandle = Input::RegisterKeyPressCallback(
      KeyCode::I, ModifierKeys::CTRL | ModifierKeys::SHIFT,
      [&]() { inspector->Open(); });
  hierarchyHandle = Input::RegisterKeyPressCallback(
      KeyCode::H, ModifierKeys::CTRL | ModifierKeys::SHIFT,
      [&]() { hierarchy->Open(); });
}
void Editor::OnDisable() {
  console->OnDisable();
  inspector->OnDisable();
  hierarchy->OnDisable();

  Input::UnregisterKeyPressCallback(
      KeyCode::M, ModifierKeys::CTRL | ModifierKeys::SHIFT, consoleHandle);
  Input::UnregisterKeyPressCallback(
      KeyCode::C, ModifierKeys::CTRL | ModifierKeys::SHIFT, consoleHandle);
  Input::UnregisterKeyPressCallback(
      KeyCode::I, ModifierKeys::CTRL | ModifierKeys::SHIFT, inspectorHandle);
  Input::UnregisterKeyPressCallback(
      KeyCode::H, ModifierKeys::CTRL | ModifierKeys::SHIFT, hierarchyHandle);
}

void Editor::GuiUpdate() {
  console->GuiUpdate();
  inspector->GuiUpdate();
  hierarchy->GuiUpdate();
  if (!menuOpen) return;

  GUI::MenuBar(
      [&]() {
        GUI::Menu("Windows", [&]() {
          if (GUI::MenuItem("Console", "Ctrl+Shift+C")) console->Open();
          if (GUI::MenuItem("Inspector", "Ctrl+Shift+I")) inspector->Open();
          if (GUI::MenuItem("Hierarchy", "Ctrl+Shift+H")) hierarchy->Open();
        });
      },
      true);
}

}  // namespace Isetta
