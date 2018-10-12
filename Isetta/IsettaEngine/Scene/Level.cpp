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
  float buttonHeight = 30;
  float buttonWidth = 200;
  float height = 80;
  float left = 200;
  float padding = 30;
  static Transform* transform = nullptr;

  for (const auto& entity : entities) {
    entity->GuiUpdate();

    if (entity->GetTransform().GetParent() == nullptr) {
      std::string extraPadding = "|--";

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
      // GUI::Text(RectTransform{Math::Rect{0, height, buttonWidth,
      // buttonHeight}}, ""); height += buttonHeight;
    }
  }

  if (transform != nullptr) {
    transform->DrawGUI();
  }
}

void Level::LateUpdate() {
  for (const auto& entity : entities) {
    entity->LastUpdate();
  }
}
}  // namespace Isetta
