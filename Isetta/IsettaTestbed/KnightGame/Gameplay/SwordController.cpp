/*
 * Copyright (c) 2018 Isetta
 */
#include "SwordController.h"

#include "KnightGame/Gameplay/KnightController.h"

using namespace Isetta;

namespace KnightGame {
void SwordController::Update() {
  auto [pos, rot] = knightMesh->GetJointWorldTransform("RightHandIndex1");
  transform->SetWorldPos(pos);
  transform->SetWorldRot(rot);
  transform->RotateWorld(Math::Vector3{20.f, 0, 0});
  transform->TranslateWorld(-0.1f * transform->GetUp());

  /// RIGHT BUMPER
  // const float speed = 5.f;
  // static float elapsed = 0;
  // static bool triggered = false;
  // if (Input::IsGamepadButtonPressed(GamepadButton::RIGHT_BUMPER)) {
  //  elapsed += speed * Time::GetDeltaTime();
  //  triggered = true;
  //} else if (triggered && elapsed < 1.f) {
  //  elapsed += speed * Time::GetDeltaTime();
  //  float angle = Math::Util::Lerp(0, 100.f, elapsed);
  //  transform->RotateLocal(-angle * Math::Vector3::forward);
  //} else if (elapsed > 0) {
  //  triggered = false;
  //  elapsed = Math::Util::Min(1.f, elapsed);
  //  elapsed -= 0.5f * speed * Time::GetDeltaTime();
  //  float angle = Math::Util::Lerp(0, 100.f, elapsed);
  //  transform->RotateLocal(-angle * Math::Vector3::forward);
  //}

  float trigger = Input::GetGamepadAxis(GamepadAxis::R_TRIGGER);
  float angle = Math::Util::Lerp(0, 100.f, trigger);
  transform->RotateLocal(-angle * Math::Vector3::forward);
  if (trigger > 0)
    knight->Walk();
  else
    knight->Run();
}
}  // namespace KnightGame
