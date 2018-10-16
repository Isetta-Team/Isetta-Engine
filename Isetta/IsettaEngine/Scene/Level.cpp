/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Level.h"
#include "Core/Math/Rect.h"
#include "Core/Memory/MemoryManager.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Scene/Entity.h"

namespace Isetta {

Entity* Level::GetEntityByName(const std::string& name) {
  StringId inID{SID(name.c_str())};
  for (const auto& entity : entities) {
    if (entity->entityID == inID) {
      return entity;
    }
  }
  LOG_WARNING(Debug::Channel::General, "Entity %s not found!", name.c_str());
  return nullptr;
}

std::list<Entity*> Level::GetEntitiesByName(const std::string& name) {
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
    entity->~Entity();
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

#if _DEBUG
  float buttonHeight = 20;
  float buttonWidth = 200;
  float height = 80;
  float left = 200;
  float padding = 20;
  static Transform* transform = nullptr;

  for (const auto& entity : entities) {
    if (entity->GetActive() && entity->GetTransform().GetParent() == nullptr) {
      Func<int, Transform*> countLevel = [](Transform* trans) -> int {
        int i = 0;
        while (trans->GetParent() != nullptr) {
          trans = trans->GetParent();
          i++;
        }
        return i;
      };

      Action<Transform*> action = [&](Transform* tran) {
        int level = countLevel(tran);
        if (GUI::Button(RectTransform{Math::Rect{left + level * padding, height,
                                                 buttonWidth - level * padding,
                                                 buttonHeight}},
                        tran->GetName())) {
          transform = transform == tran ? nullptr : tran;
        }
        height += buttonHeight;
      };

      entity->GetTransform().ForSelfAndDescendents(action);
    }
  }

  if (transform != nullptr) {
    transform->DrawGUI();
  }
#endif
}

void Level::LateUpdate() {
  for (const auto& entity : entities) {
    entity->LastUpdate();
  }
}
}  // namespace Isetta
