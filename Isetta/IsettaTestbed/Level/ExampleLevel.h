/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {
class ExampleLevel : public Level, public LevelRegistry<ExampleLevel> {
 public:
  bool IsRegisteredInLevelManager() const { return registered; }
  static inline Func<ExampleLevel*> CreateMethod = []() {
    return MemoryManager::NewOnStack<ExampleLevel>();
  };
  static std::string GetLevelName() { return "ExampleLevel"; }

 private:
  void LoadLevel() override;
};
}  // namespace Isetta
