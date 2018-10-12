/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Physics/Collider.h"

namespace Isetta {
class BoxCollider : public Collider {
 public:
  Math::Vector3 size;

  void Update() override;
};
}  // namespace Isetta
