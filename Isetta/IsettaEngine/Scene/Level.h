/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <list>
#include "ISETTA_API.h"

#define CREATE_LEVEL(NAME) class NAME : public Level, public LevelRegistry<NAME> {\
 public:\
  bool IsRegisteredInLevelManager() const { return registered; }\
  static inline Func<NAME*> CreateMethod = []() {\
    return MemoryManager::NewOnStack<NAME>();\
  };\
  static std::string GetLevelName() { return #NAME; }\
 private:

namespace Isetta {
class ISETTA_API Level {
  std::list<class Entity*> entitiesToRemove;
protected:
  std::list<class Entity*> entities;
public:
  virtual ~Level() = default;
  class Entity* GetEntityByName(const std::string&);
  class std::list<class Entity*> GetEntitiesByName(const std::string&);

  virtual void LoadLevel() {}
  virtual void UnloadLevel();

  class Entity* AddEntity(std::string name);

  void Update();
  void GUIUpdate();
  void LateUpdate();
};
}  // namespace Isetta