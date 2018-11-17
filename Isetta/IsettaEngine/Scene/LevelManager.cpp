/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/LevelManager.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Input/Input.h"
#include "Scene/Level.h"

namespace Isetta {
LevelManager& LevelManager::Instance() {
  static LevelManager instance;
  return instance;
}

bool LevelManager::Register(std::string name, Func<Level*> level) {
  levels.insert_or_assign(SID(name.c_str()), level);
  return true;
}

void LevelManager::LoadLevel() {
  if (loadLevel != nullptr) {
    UnloadLevel();
    loadedLevel = loadLevel;
    loadLevel = nullptr;
    LOG("Loading......%s", loadedLevel->GetName().c_str());
    loadedLevel->LoadLevel();
    LOG("Loading Complete");
  }
}

void LevelManager::UnloadLevel() {
  if (loadedLevel != nullptr) {
    loadedLevel->UnloadLevel();
    Input::Clear();
    LOG("Unloaded: %s", loadedLevel->GetName().c_str());
    loadedLevel->~Level();
    loadedLevel = nullptr;
  }
}

// void LevelManager::LoadStartupLevel() {
//  currentLevelName = Config::Instance().levelConfig.startLevel.GetVal();
//  LoadLevel();
//}

// void LevelManager::LoadLevel() {
//  currentLevel = levels.at(SID(currentLevelName.c_str()))();
//  if (currentLevel != nullptr) {
//    currentLevel->LoadLevel();
//  }
//}

void LevelManager::LoadLevel(std::string_view levelName) {
  loadLevel = levels.at(SID(levelName.data()))();
}

}  // namespace Isetta
