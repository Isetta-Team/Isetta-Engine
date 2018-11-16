/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/BVHTestLevel/RandomMover.h"
#include "Custom/IsettaCore.h"

namespace Isetta {
void RandomMover::Update() {
  float dt = Time::GetDeltaTime();
  GetTransform()->TranslateWorld(velocity * dt);

  coolDown -= dt;
  if (coolDown <= 0) {
    coolDown = interval;
    velocity = Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                 Math::Random::GetRandom01() - 0.5f,
                                 Math::Random::GetRandom01() - 0.5f} *
                   Math::Random::GetRandom01() * speed;

    if (range > 0 &&
        GetTransform()->GetWorldPos().SqrMagnitude() > range * range &&
        Math::Vector3::Dot(GetTransform()->GetWorldPos(), velocity) > 0) {
      velocity *= -1;
    }
  }
}
}  // namespace Isetta
