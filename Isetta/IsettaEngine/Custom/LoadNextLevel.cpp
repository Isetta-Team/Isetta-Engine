/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/LoadNextLevel.h"

#include "Core/Debug/Logger.h"
#include "Input/Input.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {
void LoadNextLevel::OnEnable() {
  handle = Input::RegisterKeyPressCallback(KeyCode::RIGHT_BRACKET, [&]() {
    LevelManager::Instance().LoadLevel(loadLevel);
    // LOG("Current Level: %s;\t Next Level.....%s",
    //    LevelManager::Instance().loadedLevel->GetName().c_str(),
    //    loadLevel.c_str());
  });
}
void LoadNextLevel::OnDisable() {
  Input::UnregisterKeyPressCallback(KeyCode::RIGHT_BRACKET, handle);
}
}  // namespace Isetta