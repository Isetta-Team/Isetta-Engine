/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_map>
#include "Core/Config/CVar.h"
#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"
#include "SID/sid.h"

namespace Isetta {
template <typename T>
class LevelRegistry {
 protected:
  static bool registered;
};

class ISETTA_API LevelManager {
 private:
  std::unordered_map<StringId, Func<class Level*>> levels;
  class Level* pendingLoadLevel{nullptr};

  bool LoadLevel();
  void UnloadLevel();
  friend class EngineLoop;

 public:
  struct LevelConfig {
    CVarString startLevel{"start_level", "Level1"};
  };

  class Level* loadedLevel{nullptr};

  static LevelManager& Instance();

  bool Register(const std::string&, Func<class Level*>);

  LevelManager() = default;
  ~LevelManager() = default;
  void LoadLevel(std::string_view levelName);
};

template <typename T>
bool LevelRegistry<T>::registered =
    LevelManager::Instance().Register(T::GetLevelName(), T::CreateMethod);
}  // namespace Isetta
