/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Level.h"
#include "Scene/Entity.h"
#include "Core/Memory/MemoryManager.h"

namespace Isetta {

Entity* Level::GetEntityByName(std::string name) {
  StringId inID{SID(name.c_str())};
  for (const auto& entity : entities) {
    if (entity->entityID == inID) {
      return entity;
    }
  }
  return nullptr;
}

std::list<Entity*> Level::GetEntitiesByName(std::string name) {
  std::list<Entity*> returnEntities;
  StringId inID{SID(name.c_str())};
  for (const auto& entity : entities) {
    if (entity->entityID == inID) {
      returnEntities.emplace_back(entity);
    }
  }
  return returnEntities;
}

void Level::UnloadLevel() {
  for (auto& entity : entities) {
    MemoryManager::FreeOnFreeList(entity);
  }
}

Entity* Level::AddEntity(std::string name) {
  Entity* entity = MemoryManager::NewOnFreeList<Entity>(name);
  entities.push_back(entity);
  return entity;
}

void Level::Update() {
  for (const auto& entity : entities) {
    entity->Update();
  }
}

void Level::GUIUpdate() {
  for (const auto& entity : entities) {
    entity->GuiUpdate();
  }
}

void Level::LateUpdate() {
  for (const auto& entity : entities) {
    entity->LastUpdate();
  }
}
}  // namespace Isetta
