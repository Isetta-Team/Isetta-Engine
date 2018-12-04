/*
 * Copyright (c) 2018 Isetta
 */
#include "LevelLoadingMenu.h"

#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Input/GLFWInput.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

#include "Graphics/Font.h"

namespace Isetta {
LevelLoadingMenu* LevelLoadingMenu::instance = nullptr;
LevelLoadingMenu::LevelLoadingMenu()
    : levels{LevelManager::Instance().GetLevelNames()} {}

void LevelLoadingMenu::OnEnable() {
  if (instance) return;
  instance = this;
  if (handle >= 0) {
    GLFWInput::UnegisterKeyCallback(handle);
  }

  handle = GLFWInput::RegisterKeyCallback(
      [&](GLFWwindow*, int key, int, int action, int) {
        if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
          LOG_INFO(Isetta::Debug::Channel::General, "Loading %d", handle);
          if (!instance) {
            GLFWInput::UnegisterKeyCallback(handle);
            Entity* entity = Entity::Instantiate("Load Level");
            entity->AddComponent<LevelLoadingMenu>();
          }
          instance->showWindow = !instance->showWindow;
        }
      });
}

void LevelLoadingMenu::OnDisable() {
  showWindow = false;
  instance = nullptr;
}

void LevelLoadingMenu::GuiUpdate() {
  GUI::Window(
      RectTransform{{20, 20, 170, 300}}, "Load Level",
      [&]() {
        const float height = 30.f;
        const float padding = 1.f;
        RectTransform rect{{0, 0, 150, height}};
        for (int i = 0; i < levels.Size(); i++) {
          if (levels[i] == LevelManager::Instance().loadedLevel->GetName())
            continue;
          if (GUI::Button(rect, levels[i].c_str()))
            LevelManager::Instance().LoadLevel(levels[i]);
          rect.rect.y += height;
        }
      },
      &showWindow);
}
}  // namespace Isetta
