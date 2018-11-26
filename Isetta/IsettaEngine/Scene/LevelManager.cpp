/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/LevelManager.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Scene/Level.h"

namespace Isetta {
LevelManager& LevelManager::Instance() {
  static LevelManager instance;
  return instance;
}

bool LevelManager::Register(const std::string_view name, Func<Level*> level) {
  levels.insert_or_assign(SID(name.data()), level);
  levelNames.push_back(name.data());
  return true;
}

std::vector<std::string> LevelManager::GetLevelNames() const {
  return levelNames;
}

void LevelManager::LoadLevel() {
  if (pendingLoadLevel != nullptr) {
    loadedLevel = pendingLoadLevel;
    pendingLoadLevel = nullptr;
    LOG("Loading......%s", loadedLevel->GetName().c_str());
    loadedLevel->OnLevelLoad();
    LOG("Loading Complete");
    loadedLevel->isLevelLoaded = true;
  }
}

void LevelManager::UnloadLevel() {
  if (loadedLevel != nullptr) {
    loadedLevel->UnloadLevel();
    LOG("Unloaded: %s", loadedLevel->GetName().c_str());
    loadedLevel->~Level();
    loadedLevel = nullptr;
  }
}

// void LevelManager::LoadStartupLevel() {
//  currentLevelName = Config::Instance().levelConfig.startLevel.GetVal();
//  OnLevelLoad();
//}

// void LevelManager::OnLevelLoad() {
//  currentLevel = levels.at(SID(currentLevelName.c_str()))();
//  if (currentLevel != nullptr) {
//    currentLevel->OnLevelLoad();
//  }
//}

void LevelManager::LoadLevel(std::string_view levelName) {
  pendingLoadLevel = levels.at(SID(levelName.data()))();
}

}  // namespace Isetta
