/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <list>

#define CREATE_LEVEL(NAME) class NAME : public Level, public LevelRegisterator<NAME> {\
 public:\
  bool IsRegisteredInLevelManager() const { return registered; }\
  static inline Func<NAME*> CreateMethod = []() {\
    return MemoryManager::NewOnStack<NAME>();\
  };\
  static std::string GetLevelName() { return #NAME; }\
 private:

namespace Isetta {
class Level {
protected:
  std::list<class Entity*> entities;
public:
  virtual ~Level() {}
  class Entity* GetEntityByName(std::string);
  class std::list<class Entity*> GetEntitiesByName(std::string);

  virtual void LoadLevel() {}
  virtual void UnloadLevel();

  // TODO(Chaojie) Change to MACRO
  class Entity* AddEntity(std::string name);

  void Update();
  void LateUpdate();
};
}  // namespace Isetta