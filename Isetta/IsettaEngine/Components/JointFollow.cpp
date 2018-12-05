/*
 * Copyright (c) 2018 Isetta
 */
#include "JointFollow.h"

#include "Graphics/MeshComponent.h"
#include "Scene/Transform.h"

namespace Isetta {
void JointFollow::Update() {
  if (!follow) return;
  auto [pos, rot] = follow->GetJointWorldTransform(jointName);
  transform->SetWorldPos(pos + posOffset);
  transform->SetWorldRot(rot);
  transform->RotateWorld(rotOffset);
}
}  // namespace Isetta
