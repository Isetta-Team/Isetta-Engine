/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Entity.h"

#include "Level.h"
#include "LevelManager.h"
#include "Scene/Component.h"
#include "Scene/Layers.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

void Entity::OnEnable() {
  PROFILE
  for (auto& comp : components) {
    comp->OnEnable();
  }
}

void Entity::GuiUpdate() {
  PROFILE
  for (auto& comp : components) {
    if (comp && comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_UPDATE)) {
      comp->GuiUpdate();
    }
  }
}

void Entity::Update() {
  PROFILE
  for (auto& comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_UPDATE)) {
      comp->Update();
    }
  }
}

void Entity::FixedUpdate() {
  PROFILE
  for (auto& comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_UPDATE)) {
      comp->FixedUpdate();
    }
  }
}

void Entity::LateUpdate() {
  PROFILE
  for (auto& comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_UPDATE)) {
      comp->LateUpdate();
    }
  }
}

void Entity::CheckDestroy() {
  PROFILE
  if (GetAttribute(EntityAttributes::NEED_DESTROY)) {
    // Destroy itself
    DestroyImmediately(this);
  } else {
    // Destroy components
    auto typeIter = componentTypes.begin();
    auto compIter = components.begin();
    while (typeIter != componentTypes.end() && compIter != components.end()) {
      Component* comp = *compIter;
      if (comp->GetAttribute(Component::ComponentAttributes::NEED_DESTROY)) {
        comp->~Component();
        comp->OnDestroy();
        MemoryManager::DeleteOnFreeList<Component>(comp);
        components.Erase(compIter);
        componentTypes.erase(typeIter);
      } else {
        ++compIter;
        ++typeIter;
      }
    }
  }
}

void Entity::OnDisable() {
  PROFILE
  for (auto& comp : components) {
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
  OnDisable();
  Destroy(this);
  CheckDestroy();
}

void Entity::Destroy(Entity* entity) {
  if (entity->GetAttribute(EntityAttributes::NEED_DESTROY)) {
    return;
  }
  if (entity->GetTransform()->GetParent()) {
    entity->GetTransform()->GetParent()->RemoveChild(&entity->transform);
  }
  DestroyHelper(entity);
}

void Entity::DestroyHelper(Entity* entity) {
  Array<Transform*> removingChildren;
  entity->SetAttribute(EntityAttributes::NEED_DESTROY, true);
  for (Transform* child : entity->transform.children) {
    removingChildren.push_back(child);
    DestroyHelper(child->GetEntity());
  }
  for (Transform* child : removingChildren) {
    entity->transform.RemoveChild(child);
  }
  entity->GetTransform()->parent = nullptr;
}

void Entity::DestroyImmediately(Entity* entity) {
  for (auto& comp : entity->components) {
    comp->OnDestroy();
  }
  for (auto& comp : entity->components) {
    MemoryManager::DeleteOnFreeList<Component>(comp);
  }
  entity->components.Clear();
  if (entity->GetTransform()->GetParent()) {
    entity->GetTransform()->GetParent()->RemoveChild(&entity->transform);
  }
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
  PROFILE
  SetAttribute(EntityAttributes::IS_TRANSFORM_DIRTY, true);
  // TODO(YIDI): Test this
  transform.SetWorldTransform(worldPos, worldEulerAngles, localScale);
}
void Entity::SetLayer(int layer) { this->layer = Layers::CheckLayer(layer); }
void Entity::SetLayer(std::string layer) {
  this->layer = Layers::NameToLayer(layer);
}
int Entity::GetLayerIndex() const { return layer; }
std::string Entity::GetLayerName() const { return Layers::LayerToName(layer); }
}  // namespace Isetta
