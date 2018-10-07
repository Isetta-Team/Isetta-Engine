/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
class LevelManager {
 public:
  class Level* currentLevel;

  static LevelManager& Instance() {
    static LevelManager instance;
    return instance;
  }

  LevelManager();
  void LoadLevel() const;
  void UnloadLevel() const;
};
}  // namespace Isetta
