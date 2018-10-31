/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Level.h"
#include <algorithm>
#include "Core/Math/Rect.h"
#include "Core/Memory/MemoryManager.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

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
  levelRoot->~Entity();
  MemoryManager::FreeOnFreeList(levelRoot);
}

void Level::AddComponentToStart(Component* component) {
  componentsToStart.push(component);
}

void Level::StartComponents() {
  while (!componentsToStart.empty()) {
    componentsToStart.top()->Start();
    componentsToStart.pop();
  }
}

Entity* Level::AddEntity(std::string name) {
  return AddEntity(name, levelRoot);
}

Entity* Level::AddEntity(std::string name, Entity* parent) {
  Entity* entity = MemoryManager::NewOnFreeList<Entity>(name);
  entities.push_back(entity);
  // TODO(YIDI): Change it when transform returns pointer
  entity->transform.SetParent(&(parent->transform));
  return entity;
}

void Level::Update() {
  StartComponents();
  for (const auto& entity : entities) {
    if (entity->GetActive())
      entity->Update();
  }
}

void Level::FixedUpdate() {
  StartComponents();
  for (const auto& entity : entities) {
    entity->FixedUpdate();
  }
}

void Level::GUIUpdate() {
  for (const auto& entity : entities) {
    entity->GuiUpdate();
  }

#if _DEBUG
  static RectTransform rectTrans{{0, 20, 200, 500}};
  bool isOpen = true;
  GUI::Window(
      rectTrans, "Heirarchy",
      [&]() {
        float buttonHeight = 20;
        float buttonWidth = 200;
        float height = 10;
        float left = 5;
        float padding = 20;
        static Transform* transform = nullptr;

        for (const auto& entity : entities) {
          Func<int, Transform*> countLevel = [](Transform* t) -> int {
            int i = 0;
            while (t->GetParent() != nullptr) {
              t = t->GetParent();
              i++;
            }
            return i;
          };

          Action<Transform*> action = [&](Transform* t) {
            int level = countLevel(t);
            if (GUI::Button(RectTransform{Math::Rect{
                                left + level * padding, height,
                                buttonWidth - level * padding, buttonHeight}},
                            t->GetName())) {
              transform = transform == t ? nullptr : t;
            }
            height += 1.25f * buttonHeight;
          };
          action(entity->GetTransform());
          entity->GetTransform()->ForDescendants(action);
        }

        if (transform != nullptr) {
          transform->InspectorGUI();
        }
      },
      NULL, GUI::WindowStyle{},
      GUI::WindowFlags::NoMove | GUI::WindowFlags::NoResize);
#endif
}

void Level::LateUpdate() {
  for (const auto& entity : entities) {
    entity->LateUpdate();
  }
  entities.remove_if([](Entity*& entity) {
    return entity->GetAttribute(Entity::EntityAttributes::NEED_DESTROY);
  });
}

Level::Level() : levelRoot{MemoryManager::NewOnFreeList<Entity>("Root")} {}
}  // namespace Isetta
