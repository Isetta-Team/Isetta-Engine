/*
 * Copyright (c) 2018 Isetta
 */
#include "LevelLoader.h"

#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Input/GLFWInput.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {
LevelLoader* LevelLoader::instance = nullptr;
LevelLoader::LevelLoader() : levels{LevelManager::Instance().GetLevelNames()} {}

void LevelLoader::OnEnable() {
  if (instance) return;
  instance = this;
  handle = GLFWInput::RegisterKeyCallback(
      [&](GLFWwindow*, int key, int, int action, int) {
        if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
          if (!instance) {
            Entity* entity = Entity::Instantiate("Load Level");
            entity->AddComponent<LevelLoader>();
          }
          instance->showWindow = !instance->showWindow;
        }
      });
}

void LevelLoader::OnDisable() {
  showWindow = false;
  instance = nullptr;
}

void LevelLoader::GuiUpdate() {
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
