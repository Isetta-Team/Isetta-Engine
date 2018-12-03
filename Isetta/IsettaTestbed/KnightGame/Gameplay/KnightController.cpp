/*
 * Copyright (c) 2018 Isetta
 */
#include "KnightController.h"

#include "KnightGame/Constants.h"

using namespace Isetta;

namespace KnightGame {
void KnightController::Start() {
  animation = entity->GetComponent<AnimationComponent>();
  death = animation->AddAnimation(KNIGHT_PATH + "death.anim", 0, "", false);
  idle = animation->AddAnimation(KNIGHT_PATH + "idle.anim", 0, "", false);
  impact = animation->AddAnimation(KNIGHT_PATH + "impact.anim", 0, "", false);
  run = animation->AddAnimation(KNIGHT_PATH + "run.anim", 0, "", false);
  walk = animation->AddAnimation(KNIGHT_PATH + "walk.anim", 0, "", false);
  animation->TransitToAnimationState(slash, 1.f);
}
void KnightController::Update() {
  Math::Vector3 movement =
      Input::GetGamepadAxis(GamepadAxis::L_HORIZONTAL) * Math::Vector3::left +
      Input::GetGamepadAxis(GamepadAxis::L_VERTICAL) * Math::Vector3::forward;
  float moveMag = movement.Magnitude();
  if (moveMag > 0.0f) movement /= moveMag;

  float t = 0;
  if (canRun && moveMag > runAxis) {
    t = 1;
    animation->TransitToAnimationState(run, 0.1f);
  } else if (canRun && moveMag > walkAxis) {
    const float div = 1.f / (runAxis - walkAxis);
    t = (moveMag - walkAxis) / div;
  } else if (moveMag != 0.f) {
    animation->TransitToAnimationState(walk, 0.1f);
  } else {
    animation->TransitToAnimationState(idle, 0.1f);
  }
  float speed = Math::Util::Lerp(walkSpeed, runSpeed, t);

  if (transform->GetWorldPos().z > depth && movement.z > 0)
    movement.z = 0;
  else if (transform->GetWorldPos().z < -depth && movement.z < 0)
    movement.z = 0;

  transform->TranslateWorld(movement * speed * Time::GetDeltaTime());
  if (movement.x != 0)
    transform->LookAt(transform->GetWorldPos() +
                      -movement.x * Math::Vector3::right);
}
}  // namespace KnightGame