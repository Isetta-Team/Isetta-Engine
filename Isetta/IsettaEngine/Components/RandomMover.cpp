/*
 * Copyright (c) 2018 Isetta
 */
#include "Components/RandomMover.h"
#include "Custom/IsettaCore.h"

namespace Isetta {
void RandomMover::Update() {
  float dt = Time::GetDeltaTime();
  GetTransform().TranslateWorld(velocity * dt);

  coolDown -= dt;
  if (coolDown <= 0) {
    coolDown = interval;
    velocity = Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                 Math::Random::GetRandom01() - 0.5f,
                                 Math::Random::GetRandom01() - 0.5f} *
                   Math::Random::GetRandom01() * speed;
  }
}
}  // namespace Isetta
