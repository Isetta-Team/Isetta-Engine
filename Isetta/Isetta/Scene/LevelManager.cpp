/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Level.h"
#include "Scene/LevelManager.h"
#include "Custom/ExampleLevel.h"
#include "Core/Memory/MemoryManager.h"

namespace Isetta {
LevelManager::LevelManager() {
  // TODO(Chaojie): Find a way to register a level
  currentLevel = MemoryManager::NewOnStack<ExampleLevel>();
}

void LevelManager::LoadLevel() const {
  if (currentLevel != nullptr) {
    currentLevel->LoadLevel();
  }
}

void LevelManager::UnloadLevel() const {
  if (currentLevel != nullptr) {
    currentLevel->UnloadLevel();
  }
}
}
