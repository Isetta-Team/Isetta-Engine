/*
 * Copyright (c) 2018 Isetta
 */
#include "EditorComponent.h"

#include "Graphics/GUI.h"
#include "Input/Input.h"
#include "Input/KeyCode.h"
#include "Scene/Entity.h"

#include "Components/Editor/Console.h"
#include "Components/Editor/FrameReporter.h"
#include "Components/Editor/Hierarchy.h"
#include "Components/Editor/Inspector.h"

namespace Isetta {
#ifdef _EDITOR
void EditorComponent::Awake() {
  console = entity->AddComponent<Console>("Console", true);
  inspector = entity->AddComponent<Inspector>("Inspector", false, true);
  hierarchy = entity->AddComponent<Hierarchy>("Hierarchy", true, inspector);
  frameReporter = entity->AddComponent<FrameReporter>();
}
void EditorComponent::OnEnable() {
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
  frameReporterHandle = Input::RegisterKeyPressCallback(
      KeyCode::F, ModifierKeys::CTRL | ModifierKeys::SHIFT,
      [&]() { frameReporter->Open(); });
}
void EditorComponent::OnDisable() {
  Input::UnregisterKeyPressCallback(
      KeyCode::M, ModifierKeys::CTRL | ModifierKeys::SHIFT, consoleHandle);
  Input::UnregisterKeyPressCallback(
      KeyCode::C, ModifierKeys::CTRL | ModifierKeys::SHIFT, consoleHandle);
  Input::UnregisterKeyPressCallback(
      KeyCode::I, ModifierKeys::CTRL | ModifierKeys::SHIFT, inspectorHandle);
  Input::UnregisterKeyPressCallback(
      KeyCode::H, ModifierKeys::CTRL | ModifierKeys::SHIFT, hierarchyHandle);
  Input::UnregisterKeyPressCallback(KeyCode::F,
                                    ModifierKeys::CTRL | ModifierKeys::SHIFT,
                                    frameReporterHandle);
}

void EditorComponent::GuiUpdate() {
  if (!menuOpen) return;

  GUI::MenuBar(
      [&]() {
        GUI::Menu("Windows", [&]() {
          if (GUI::MenuItem("Console", "Ctrl+Shift+C")) console->Open();
          if (GUI::MenuItem("Inspector", "Ctrl+Shift+I")) inspector->Open();
          if (GUI::MenuItem("Hierarchy", "Ctrl+Shift+H")) hierarchy->Open();
          if (GUI::MenuItem("Frame Reporter", "Ctrl+Shift+F"))
            frameReporter->Open();
        });
      },
      true);
}
#endif
}  // namespace Isetta
