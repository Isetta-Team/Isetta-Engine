/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <list>
#include <queue>
#include <set>
#include <string>
#include "Core/Memory/TemplatePoolAllocator.h"
#include "ISETTA_API.h"

#define DEFINE_LEVEL(NAME)                                                \
  class NAME : public Isetta::Level, public Isetta::LevelRegistry<NAME> { \
   public:                                                                \
    bool IsRegisteredInLevelManager() const { return registered; }        \
    static inline Isetta::Func<NAME*> CreateMethod = []() {               \
      return Isetta::MemoryManager::NewOnStack<NAME>();                   \
    };                                                                    \
    static std::string GetLevelName() { return #NAME; }                   \
    std::string GetName() const override { return #NAME; }                \
                                                                          \
   private:

#define DEFINE_LEVEL_END \
  }                      \
  ;

namespace Isetta {
class ISETTA_API Level {
 private:
  std::list<class Entity*> entitiesToRemove;
  void AddComponentToStart(class Component* component);
  void StartComponents();

  class Entity* AddEntity(std::string name, class Entity* parent,
                          bool entityStatic = false);

  void Unload();
  void Update();
  void GUIUpdate();
  void FixedUpdate();
  void LateUpdate();

  bool isLevelLoaded = false;

  TemplatePoolAllocator<Entity> pool;

  friend class Entity;
  friend class EngineLoop;
  friend class GUIModule;
  friend class LevelManager;

 protected:
  std::list<class Entity*> entities;
  std::queue<class Component*> componentsToStart;
  std::set<class Component*> componentsToDestroy;

 public:
  class Entity* levelRoot;
  Level();
  virtual ~Level() = default;

  virtual std::string GetName() const = 0;
  /**
   * \brief Get all entities in the level
   */
  class std::list<class Entity*> GetEntities() const;
  /**
   * \brief Get entitiy by name in the level, if multiple will return first
   * found
   */
  class Entity* GetEntityByName(const std::string_view);
  /**
   * \brief Get all entities with the name in the level
   */
  class std::list<class Entity*> GetEntitiesByName(const std::string_view);

  /**
   * \brief This is where we put our "level loading script". This function is
   * called when the level is loaded, so this is where you should
   * spawn/initialize your entities and layout your level
   */
  virtual void Load() = 0;

  /**
   * \brief Called the this level has finished unloading
   */
  virtual void OnUnload() {}
  /**
   * \brief Check if the level is loaded
   */
  bool IsLevelLoaded() const;
};
}  // namespace Isetta
