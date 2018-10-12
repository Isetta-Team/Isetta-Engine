/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Physics/Collider.h"

namespace Isetta {
class SphereCollider : public Collider {
 public:
  float radius;

  void Update() override;
};
}  // namespace Isetta
