/*
 * Copyright (c) 2018 Isetta
 */
#include "AI/Nav2DAgent.h"

namespace Isetta {
Math::Vector2 Nav2DAgent::GetAIMovement(
    Math::Vector2 agentPosition, float deltaTime) {
  UpdateSteeringData(agentPosition);
  return GetAIMovement(deltaTime);;
}

Math::Vector2 Nav2DAgent::GetAIMovement(float deltaTime) {
  velocity += linear * deltaTime;
  if (velocity.Magnitude() > maxVelocity) {
    velocity.Normalize();
    velocity *= maxVelocity;
  }
  return velocity;
}

void Nav2DAgent::UpdateSteeringData(Math::Vector2 position) {
  Math::Vector2 targetVelocity = navPlane->GetDirectionByPosition(position);
  linear = targetVelocity - velocity;
  linear /= timeToTarget;
  if (linear.Magnitude() > maxAcceleration) {
    linear.Normalize();
    linear *= maxAcceleration;
  }
}

Isetta::Nav2DAgent::Nav2DAgent(Nav2DPlane* plane) : navPlane{plane} {}
}  // namespace Isetta
