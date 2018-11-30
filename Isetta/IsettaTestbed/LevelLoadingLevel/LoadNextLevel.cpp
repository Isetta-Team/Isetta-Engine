/*
 * Copyright (c) 2018 Isetta
 */
#include "LoadNextLevel.h"

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