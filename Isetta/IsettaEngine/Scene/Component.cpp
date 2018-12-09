/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace Isetta {

bool Component::isFlattened = false;

bool Component::RegisterComponent(std::type_index curr, std::type_index base,
                                  bool isUnique) {
  if (isUnique) uniqueComponents().insert(curr);
  std::unordered_map<std::type_index, std::list<std::type_index>>& children =
      childrenTypes();

  if (children.count(curr) > 0) {
    children.at(curr).push_front(curr);
  } else {
    children.insert({curr, std::list<std::type_index>{curr}});
  }

  if (children.count(base) > 0) {
    children.at(base).emplace_back(curr);
  } else {
    children.insert({base, std::list<std::type_index>{curr}});
  }
  return true;
}

void Component::FlattenComponentList() {
  if (isFlattened) return;
  isFlattened = true;
  std::unordered_map<std::type_index, std::list<std::type_index>>& children =
      childrenTypes();
  std::type_index componentIndex{typeid(Component)};
  std::list<std::type_index>* componentList = &children.at(componentIndex);
  for (auto& childList : *componentList) {
    if (childList != componentIndex) {
      FlattenHelper(componentIndex, childList);
    }
  }
}

void Component::FlattenHelper(std::type_index parent, std::type_index curr) {
  std::unordered_map<std::type_index, std::list<std::type_index>>& children =
      childrenTypes();
  std::list<std::type_index>* parentList = &children.at(parent);
  std::list<std::type_index>* componentList = &children.at(curr);
  for (auto& childList : *componentList) {
    if (childList != curr) {
      parentList->push_back(childList);
      FlattenHelper(curr, childList);
    }
  }
}

Component::Component()
    : attributes{0b1111001}, entity{nullptr}, transform(nullptr) {
  if (!isFlattened) {
    FlattenComponentList();
  }
}

void Component::SetAttribute(ComponentAttributes attr, bool value) {
  attributes.set(static_cast<int>(attr), value);
}

bool Component::GetAttribute(ComponentAttributes attr) const {
  return attributes.test(static_cast<int>(attr));
}

void Component::SetActive(bool value) {
  bool isActive = GetAttribute(ComponentAttributes::IS_ACTIVE);
  SetAttribute(ComponentAttributes::IS_ACTIVE, value);
  if (!isActive && value) {
    if (!GetAttribute(ComponentAttributes::HAS_AWAKEN)) {
      Awake();
      SetAttribute(ComponentAttributes::HAS_AWAKEN, true);
    }
    OnEnable();
  } else if (isActive && !value &&
             GetAttribute(ComponentAttributes::HAS_AWAKEN)) {
    OnDisable();
  }
}

bool Component::GetActive() const {
  return GetAttribute(ComponentAttributes::IS_ACTIVE);
}

void Component::Destroy(Component* component) {
  component->SetAttribute(ComponentAttributes::NEED_DESTROY, true);
}
}  // namespace Isetta
