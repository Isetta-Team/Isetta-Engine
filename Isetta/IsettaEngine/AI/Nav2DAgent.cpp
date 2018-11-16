/*
 * Copyright (c) 2018 Isetta
 */
#include "AI/Nav2DAgent.h"

namespace Isetta {
Math::Vector2 Nav2DAgent::GetAIMovement(Math::Vector2 agentPosition,
                                        float deltaTime) {
  float distance = navPlane->GetDistanceToTarget(agentPosition);

  UpdateSteeringData(agentPosition, distance);
  Math::Vector2 velocity{0.f};
  if (distance < arrivingDistance) {
    velocity = Math::Vector2::zero;
  } else {
    velocity = GetAIMovement(deltaTime);
  }

  return velocity;
}

Math::Vector2 Nav2DAgent::GetAIMovement(float deltaTime) {
  velocity += linear * deltaTime;
  if (velocity.Magnitude() > maxVelocity) {
    velocity.Normalize();
    velocity *= maxVelocity;
  }
  return velocity;
}

Nav2DAgent::Nav2DAgent(Nav2DPlane* plane, float maxAcc, float maxV,
                       float timeToTarget, float arrving)
    : navPlane{plane},
      maxAcceleration{maxAcc},
      maxVelocity{maxV},
      timeToTarget{timeToTarget},
      arrivingDistance{arrving} {}

void Nav2DAgent::UpdateSteeringData(Math::Vector2 position, float distance) {
  float targetSpeed{0};
  if (distance > arrivingDistance * 2) {
    targetSpeed = maxVelocity;
  } else {
    targetSpeed = maxVelocity * distance / (arrivingDistance * 2);
  }
  Math::Vector2 targetVelocity =
      navPlane->GetDirectionByPosition(position) * targetSpeed;
  linear = targetVelocity - velocity;
  linear /= timeToTarget;
  if (linear.Magnitude() > maxAcceleration) {
    linear.Normalize();
    linear *= maxAcceleration;
  }
}
}  // namespace Isetta
