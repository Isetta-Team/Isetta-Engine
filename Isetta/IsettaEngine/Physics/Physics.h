/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Scene/Transform.h"

namespace Isetta {
class Physics : public Component {
 private:
  bool isTrigger, isStatic;  // TODO(Jacob) make private, getter/setter
  std::vector<class Collider*> colliders;
};
}  // namespace Isetta
