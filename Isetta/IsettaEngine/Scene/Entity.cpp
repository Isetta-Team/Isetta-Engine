/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Entity.h"
#include "Level.h"
#include "LevelManager.h"
#include "Scene/Component.h"

namespace Isetta {

void Entity::OnEnable() {
  for (auto comp : components) {
    comp->OnEnable();
  }
}

void Entity::CheckStart() {
  for (auto comp : components) {
    if (comp->GetActive() &&
        !comp->GetAttribute(Component::ComponentAttributes::HAS_STARTED)) {
      comp->SetAttribute(Component::ComponentAttributes::HAS_STARTED, true);
      comp->Start();
    }
  }
}

void Entity::GuiUpdate() {
  for (auto comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_UPDATE)) {
      comp->GuiUpdate();
    }
  }
}

void Entity::Update() {
  CheckStart();
  for (auto comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_UPDATE)) {
      comp->Update();
    }
  }
}

void Entity::LastUpdate() {
  for (auto comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_UPDATE)) {
      comp->LateUpdate();
    }
  }
  // SetAttribute(EntityAttributes::IS_TRANSFORM_DIRTY, false);
  CheckDestroy();
}

void Entity::CheckDestroy() {
  if (GetAttribute(EntityAttributes::NEED_DESTROY)) {
    // Destroy itself
    for (auto comp : components) {
      comp->OnDestroy();
    }
    for (auto comp : components) {
      comp->~Component();
      MemoryManager::FreeOnFreeList(comp);
    }
    // TODO(Chaojie): delete child
  } else {
    // Destroy components
    auto typeIter = componentTypes.begin();
    auto compIter = components.begin();
    while (typeIter != componentTypes.end() && compIter != components.end()) {
      Component* comp = *compIter;
      if (comp->GetAttribute(Component::ComponentAttributes::NEED_DESTROY)) {
        comp->OnDestroy();
        MemoryManager::FreeOnFreeList(comp);
        components.erase(compIter);
        componentTypes.erase(typeIter);
      } else {
        ++compIter;
        ++typeIter;
      }
    }
  }
  // TODO(Chaojie) unregister from level
}

void Entity::OnDisable() {
  for (auto comp : components) {
    comp->OnDisable();
  }
}

void Entity::SetAttribute(EntityAttributes attr, bool value) {
  attributes.set(static_cast<int>(attr), value);
}

bool Entity::GetAttribute(EntityAttributes attr) const {
  return attributes.test(static_cast<int>(attr));
}

Entity::Entity(const std::string& name)
    : transform(this),
      attributes{0b101},
      entityID{SID(name.c_str())},
      entityName{name} {
  OnEnable();
}

Entity::~Entity() {
  LOG_INFO(Debug::Channel::Gameplay, "Entity destroyed: %s", entityName.c_str());
  OnDisable();
  Destroy(this);
  CheckDestroy();
}

void Entity::Destroy(Entity* entity) {
  entity->SetAttribute(EntityAttributes::NEED_DESTROY, true);
}

Entity* Entity::GetEntityByName(const std::string& name) {
  return LevelManager::Instance().currentLevel->GetEntityByName(name);
}

std::list<Entity*> Entity::GetEntitiesByName(const std::string& name) {
  return LevelManager::Instance().currentLevel->GetEntitiesByName(name);
}

void Entity::SetActive(bool inActive) {
  bool isActive = GetAttribute(EntityAttributes::IS_ACTIVE);
  SetAttribute(EntityAttributes::IS_ACTIVE, inActive);
  if (!isActive && inActive) {
    OnEnable();
  } else if (isActive && !inActive) {
    OnDisable();
  }
}

bool Entity::GetActive() const {
  return GetAttribute(EntityAttributes::IS_ACTIVE);
}

void Entity::SetTransform(const Math::Vector3& worldPos,
                          const Math::Vector3& worldEulerAngles,
                          const Math::Vector3& localScale) {
  SetAttribute(EntityAttributes::IS_TRANSFORM_DIRTY, true);
  // TODO(YIDI): Test this
  transform.SetWorldTransform(worldPos, worldEulerAngles, localScale);
}
}  // namespace Isetta
