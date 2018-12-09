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
  for (auto &comp : components) {
    comp->OnEnable();
  }
}

void Entity::GuiUpdate() {
  PROFILE
  for (auto &comp : components) {
    if (comp && comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_GUI_UPDATE)) {
      comp->GuiUpdate();
    }
  }
}

void Entity::Update() {
  PROFILE
  for (auto &comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_UPDATE)) {
      comp->Update();
    }
  }
}

void Entity::FixedUpdate() {
  PROFILE
  for (auto &comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_FIXED_UPDATE)) {
      comp->FixedUpdate();
    }
  }
}

void Entity::LateUpdate() {
  PROFILE
  for (auto &comp : components) {
    if (comp->GetActive() &&
        comp->GetAttribute(Component::ComponentAttributes::NEED_LATE_UPDATE)) {
      comp->LateUpdate();
    }
  }
  CheckDestroy();
}

void Entity::CheckDestroy() {
  PROFILE
  if (GetAttribute(EntityAttributes::NEED_DESTROY)) {
    // Destroy itself
    if (GetActive()) {
      OnDisable();
    }
    DestroyImmediately(this);
  } else {
    // Destroy components
    auto typeIter = componentTypes.begin();
    auto compIter = components.begin();
    while (typeIter != componentTypes.end() && compIter != components.end()) {
      Component *comp = *compIter;
      if (comp->GetAttribute(Component::ComponentAttributes::NEED_DESTROY)) {
        if (comp->GetActive()) comp->OnDisable();
        comp->OnDestroy();
        comp->~Component();
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
  for (auto &comp : components) {
    if (comp->GetActive()) comp->OnDisable();
  }
}

void Entity::SetAttribute(EntityAttributes attr, bool value) {
  attributes.set(static_cast<int>(attr), value);
}

bool Entity::GetAttribute(EntityAttributes attr) const {
  return attributes.test(static_cast<int>(attr));
}

Entity::Entity(const std::string &name, const bool entityStatic)
    : internalTransform(this),
      attributes{0b101},
      entityName{name},
      transform(&internalTransform),
      isStatic{entityStatic} {
  CoCreateGuid(&entityId);
  OnEnable();
}

Entity::~Entity() {
  Destroy(this);
  CheckDestroy();
}

bool Entity::operator==(const Entity &rhs) const {
  return entityId == rhs.entityId;
}

std::string Entity::GetEntityIdString() const {
  std::array<char, 40> output;
  snprintf(output.data(), output.size(),
           "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}", entityId.Data1,
           entityId.Data2, entityId.Data3, entityId.Data4[0], entityId.Data4[1],
           entityId.Data4[2], entityId.Data4[3], entityId.Data4[4],
           entityId.Data4[5], entityId.Data4[6], entityId.Data4[7]);

  return std::string(output.data());
}

Entity *Entity::Instantiate(const std::string name, Entity *parent,
                            const bool entityStatic) {
  return LevelManager::Instance().loadedLevel->AddEntity(name, parent,
                                                         entityStatic);
}

void Entity::Destroy(Entity *entity) {
  if (entity->GetAttribute(EntityAttributes::NEED_DESTROY)) {
    return;
  }
  if (entity->transform->GetParent()) {
    entity->transform->GetParent()->RemoveChild(entity->transform);
  }
  DestroyHelper(entity);
}

void Entity::DestroyHelper(Entity *entity) {
  Array<Transform *> removingChildren;
  entity->SetAttribute(EntityAttributes::NEED_DESTROY, true);
  for (Transform *child : entity->transform->children) {
    removingChildren.PushBack(child);
    DestroyHelper(child->entity);
  }
  for (Transform *child : removingChildren) {
    entity->transform->RemoveChild(child);
  }
  entity->transform->parent = nullptr;
}

void Entity::DestroyImmediately(Entity *entity) {
  for (auto &comp : entity->components) {
    comp->OnDestroy();
  }
  for (auto &comp : entity->components) {
    MemoryManager::DeleteOnFreeList<Component>(comp);
  }
  entity->components.Clear();
  if (entity->transform->GetParent()) {
    entity->transform->GetParent()->RemoveChild(entity->transform);
  }
}

Entity *Entity::GetEntityByName(const std::string &name) {
  return LevelManager::Instance().loadedLevel->GetEntityByName(name);
}

std::list<Entity *> Entity::GetEntitiesByName(const std::string &name) {
  return LevelManager::Instance().loadedLevel->GetEntitiesByName(name);
}

void Entity::SetActive(const bool inActive) {
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

bool Entity::IsMoveable() const {
  return !isStatic || !LevelManager::Instance().loadedLevel->IsLevelLoaded();
}

void Entity::SetTransform(const Math::Vector3 &worldPos,
                          const Math::Vector3 &worldEulerAngles,
                          const Math::Vector3 &localScale) {
  PROFILE
  SetAttribute(EntityAttributes::IS_TRANSFORM_DIRTY, true);
  transform->SetWorldTransform(worldPos, worldEulerAngles, localScale);
}
void Entity::SetLayer(const int layer) {
  this->layer = Layers::CheckLayer(layer);
}
void Entity::SetLayer(const std::string layer) {
  this->layer = Layers::NameToLayer(layer);
}
int Entity::GetLayerIndex() const { return layer; }
std::string Entity::GetLayerName() const { return Layers::LayerToName(layer); }
}  // namespace Isetta
