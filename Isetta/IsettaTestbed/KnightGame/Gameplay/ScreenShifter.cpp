/*
 * Copyright (c) 2018 Isetta
 */
#include "ScreenShifter.h"

#include "Collisions/Collider.h"
#include "Core/Time/Time.h"
#include "Scene/Transform.h"

using namespace Isetta;

namespace KnightGame {
void ScreenShifter::Update() {
  transform->TranslateWorld(-Math::Vector3::right * speed *
                            Time::GetDeltaTime());
}
}  // namespace KnightGame
