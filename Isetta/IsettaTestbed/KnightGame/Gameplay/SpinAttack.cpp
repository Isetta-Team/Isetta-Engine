/*
 * Copyright (c) 2018 Isetta
 */
#include "SpinAttack.h"

using namespace Isetta;

namespace KnightGame {
void SpinAttack::Update() {
  // if (!callbackInProgress) {
  Math::Vector2 spin =
      Input::GetGamepadAxis(GamepadAxis::R_HORIZONTAL) * Math::Vector2::left +
      Input::GetGamepadAxis(GamepadAxis::R_VERTICAL) * Math::Vector2::up;
  float spinMag = spin.Magnitude();
  if (spinMag < axisMin) {
    extended = started = false;
    return;
  }

  spin /= spinMag;
  float angle = Math::Util::Atan2(spin.y, spin.x);
  elapsedTime += Time::GetDeltaTime();
  if (!started && !extended) {
    lastAngle = startAngle = angle;
    direction = elapsedTime = 0;
    extended = started = true;
  } else if (started) {
    float delta = angle - lastAngle;
    if (Math::Util::Abs(delta) < anglePercision) {
      if (direction != 0 &&
          Math::Util::Abs(angle - startAngle) < anglePercision) {
        LOG("COMPLETED");
        callbackInProgress = true;
        callback();
        started = false;
        direction = 0;
        elapsedTime = 0;
      }
    } else if (delta > 0) {
      if (direction == -1)
        started = false;
      else
        direction = 1;
    } else {
      if (direction == 1)
        started = false;
      else
        direction = -1;
    }
    lastAngle = angle;
  }

  if (started && elapsedTime > timeToComplete) {
    // LOG("Failed");
    started = false;
  }
  //}
}

void SpinAttack::SetCallback(Action<> callback) { this->callback = callback; }
}  // namespace KnightGame
