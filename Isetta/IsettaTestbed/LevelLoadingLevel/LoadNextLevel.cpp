/*
 * Copyright (c) 2018 Isetta
 */
#include "LoadNextLevel.h"

#include "Core/Debug/Logger.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {
void LoadNextLevel::GuiUpdate() {
  if (GUI::Button(
          RectTransform{
              {0, 0, 500, 100}, GUI::Pivot::Center, GUI::Pivot::Center},
          "Load Level: " + loadLevel)) {
    // Load level named loadLevel
    LevelManager::Instance().LoadLevel(loadLevel);
    LOG("Current Level: %s;\t Next Level.....%s",
        LevelManager::Instance().loadedLevel->GetName().c_str(),
        loadLevel.c_str());
  }
}
}  // namespace Isetta