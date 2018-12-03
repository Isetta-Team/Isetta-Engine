/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Level.h"
#include "Audio/AudioModule.h"
#include "Core/Config/Config.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

Entity* Level::GetEntityByName(const std::string_view name) {
  for (const auto& entity : entities) {
    if (entity->entityName == name) {
      return entity;
    }
  }
  LOG_WARNING(Debug::Channel::General, "Entity %s not found!", name.data());
  return nullptr;
}

std::list<Entity*> Level::GetEntitiesByName(const std::string_view name) {
  std::list<Entity*> returnEntities;
  for (const auto& entity : entities) {
    if (entity->entityName == name) {
      returnEntities.emplace_back(entity);
    }
  }
  return returnEntities;
}

bool Level::IsLevelLoaded() const { return isLevelLoaded; }

std::list<class Entity*> Level::GetEntities() const { return entities; }

void Level::Unload() {
  PROFILE
  OnUnload();
  pool.Free(levelRoot);
  for (auto& entity : entities) {
    pool.Free(entity);
    entity = nullptr;
  }
}

void Level::AddComponentToStart(Component* component) {
  componentsToStart.push(component);
}

void Level::StartComponents() {
  PROFILE
  while (!componentsToStart.empty()) {
    componentsToStart.front()->Start();
    componentsToStart.pop();
  }
}

Entity* Level::AddEntity(std::string name, Entity* parent, bool entityStatic) {
  PROFILE
  Entity* entity = pool.Get(name, entityStatic);
  entity->transform->SetParent(parent != nullptr ? parent->transform
                                                 : levelRoot->transform);
  entities.push_back(entity);
  return entity;
}

void Level::Update() {
  BROFILER_CATEGORY("Level Update", Profiler::Color::GoldenRod);

  StartComponents();
  for (const auto& entity : entities) {
    if (entity->GetActive()) entity->Update();
  }
}

void Level::FixedUpdate() {
  BROFILER_CATEGORY("Level Fixed Update", Profiler::Color::DarkSeaGreen);

  StartComponents();
  for (const auto& entity : entities) {
    entity->FixedUpdate();
  }
}

void Level::GUIUpdate() {
  PROFILE
  for (const auto& entity : entities) {
    entity->GuiUpdate();
  }
}

void Level::LateUpdate() {
  BROFILER_CATEGORY("Level Late Update", Profiler::Color::LightCyan);

  for (const auto& entity : entities) {
    entity->LateUpdate();
  }

  for (auto& entity : entities) {
    if (entity->GetAttribute(Entity::EntityAttributes::NEED_DESTROY)) {
      pool.Free(entity);
      entity = nullptr;
    }
  }

  entities.remove_if([](Entity*& entity) { return entity == nullptr; });
}

Level::Level()
    : pool(CONFIG_VAL(memoryConfig.entityPoolInitialSize),
           CONFIG_VAL(memoryConfig.entityPoolIncrement)),
      levelRoot(pool.Get("Root")) {}
}  // namespace Isetta
