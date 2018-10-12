/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector3.h"
#include "Physics/AABB.h"
#include "Scene/Component.h"

namespace Isetta {
class Collider : public Component {
 public:
  bool isTrigger, isStatic;
  Math::Vector3 center;  // TODO(JACOB) remove

  void OnEnable() override;
  void OnDisable() override;

 protected:
  AABB* bounding;

  static class PhysicsModule* physicsModule;
  friend class PhysicsModule;
};
}  // namespace Isetta
