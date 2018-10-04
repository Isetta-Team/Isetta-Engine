/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Component.h"

namespace Isetta {

Component::Component() : owner{nullptr}, attributes{0b1001} {}

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
    OnEnable();
  } else if (isActive && !value) {
    OnDisable();
  }
}

bool Component::GetActive() const {
  return GetAttribute(ComponentAttributes::IS_ACTIVE);
}

}  // namespace Isetta
