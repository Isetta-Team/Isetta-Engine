/*
 * Copyright (c) 2018 Isetta
 */
#include "FollowComponent.h"

using namespace Isetta;

namespace KnightGame {
void FollowComponent::LateUpdate() {
  Math::Vector3 position = transform->GetWorldPos();
  Math::Vector3 followPos = follow->transform->GetWorldPos();
  for (int i = 0; i < 3; i++)
    if (direction[i]) position[i] = followPos[i];
  transform->SetWorldPos(position);
}
}  // namespace KnightGame
