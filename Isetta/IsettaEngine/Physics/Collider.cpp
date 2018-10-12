/*
 * Copyright (c) 2018 Isetta
 */
#include "Physics/Collider.h"
#include "Physics/PhysicsModule.h"

namespace Isetta {
PhysicsModule* Collider::physicsModule{nullptr};
void Collider::OnEnable() { physicsModule->tree.Add(bounding); }
void Collider::OnDisable() {}
}  // namespace Isetta
