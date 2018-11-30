/*
 * Copyright (c) 2018 Isetta
 */
#include "RandomMover.h"

namespace Isetta {
void RandomMover::Update() {
  float dt = Time::GetDeltaTime();
  transform->TranslateWorld(velocity * dt);

  coolDown -= dt;
  if (coolDown <= 0) {
    coolDown = interval;
    velocity = Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                             Math::Random::GetRandom01() - 0.5f,
                             Math::Random::GetRandom01() - 0.5f} *
               Math::Random::GetRandom01() * speed;

    if (range > 0 && transform->GetWorldPos().SqrMagnitude() > range * range &&
        Math::Vector3::Dot(transform->GetWorldPos(), velocity) > 0) {
      velocity *= -1;
    }
  }
}
}  // namespace Isetta
