/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_map>
#include "SID/sid.h"
#include "imgui_impl_opengl3.h"

namespace Isetta {
template <typename T>
class LevelRegisterator {
 protected:
  static bool registered;
};

class LevelManager {
 private:
  std::unordered_map<StringId, Func<class Level*>> levels;

 public:
  std::string currentLevelName;
  class Level* currentLevel;

  static LevelManager& Instance();

  bool Register(std::string, Func<class Level*>);

  LevelManager();
  void LoadLevel();
  void UnloadLevel() const;
  ~LevelManager();
};

template <typename T>
bool LevelRegisterator<T>::registered =
    LevelManager::Instance().Register(T::GetLevelName(), T::CreateMethod);
}  // namespace Isetta
