/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/LevelManager.h"
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Core/Config/Config.h"

namespace Isetta {
LevelManager& LevelManager::Instance() {
  static LevelManager instance;
  return instance;
}

bool LevelManager::Register(std::string name, Func<Level*> level) {
  levels.insert_or_assign(SID(name.c_str()), level);
  return true;
}

void LevelManager::LoadStartupLevel() {
  currentLevelName = Config::Instance().levelConfig.startLevel.GetVal();
  LoadLevel();
}

void LevelManager::LoadLevel() {
   currentLevel = levels.at(SID(currentLevelName.c_str()))();
  if (currentLevel != nullptr) {
    currentLevel->LoadLevel();
  }
}

void LevelManager::UnloadLevel() const {
  if (currentLevel != nullptr) {
    currentLevel->UnloadLevel();
  }
}

}  // namespace Isetta
