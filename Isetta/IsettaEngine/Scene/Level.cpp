/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Level.h"
#include "Core/Memory/MemoryManager.h"
#include "Graphics/GUI.h"
#include "Input/Input.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

Entity* Level::GetEntityByName(const std::string& name) {
  for (const auto& entity : entities) {
    if (entity->entityName == name) {
      return entity;
    }
  }
  LOG_WARNING(Debug::Channel::General, "Entity %s not found!", name.c_str());
  return nullptr;
}

std::list<Entity*> Level::GetEntitiesByName(const std::string& name) {
  std::list<Entity*> returnEntities;
  for (const auto& entity : entities) {
    if (entity->entityName == name) {
      returnEntities.emplace_back(entity);
    }
  }
  return returnEntities;
}

std::list<class Entity*> Level::GetEntities() const { return entities; }

void Level::UnloadLevel() {
  PROFILE
  // for (auto& entity : entities) {
  //   if (entity->GetTransform()->GetParent() == levelRootTransform) {
  //     MemoryManager::DeleteOnFreeList<Entity>(entity);
  //   }
  // }
  MemoryManager::DeleteOnFreeList<Entity>(levelRoot);
  for (auto& entity : entities) {
    MemoryManager::DeleteOnFreeList<Entity>(entity);
    entity = nullptr;
  }
  Input::Clear();
}

void Level::AddComponentToStart(Component* component) {
  componentsToStart.push(component);
}

void Level::StartComponents() {
  PROFILE
  while (!componentsToStart.empty()) {
    componentsToStart.top()->Start();
    componentsToStart.pop();
  }
}

Entity* Level::AddEntity(std::string name, bool entityStatic) {
  return AddEntity(name, levelRoot, entityStatic);
}

Entity* Level::AddEntity(std::string name, Entity* parent, bool entityStatic) {
  PROFILE
  Entity* entity = MemoryManager::NewOnFreeList<Entity>(name, entityStatic);
  entities.push_back(entity);
  // TODO(YIDI): Change it when transform returns pointer
  entity->transform->SetParent(parent->transform);
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
      MemoryManager::DeleteOnFreeList<Entity>(entity);
      entity = nullptr;
    }
  }

  entities.remove_if([](Entity*& entity) { return entity == nullptr; });
}

Level::Level() : levelRoot{MemoryManager::NewOnFreeList<Entity>("Root")} {}
}  // namespace Isetta
