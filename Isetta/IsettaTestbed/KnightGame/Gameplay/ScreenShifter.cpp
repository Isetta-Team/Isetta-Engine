/*
 * Copyright (c) 2018 Isetta
 */
#include "ScreenShifter.h"

using namespace Isetta;

namespace KnightGame {
void ScreenShifter::Update() {
  transform->TranslateWorld(Math::Vector3::left * speed * Time::GetDeltaTime());
}
}  // namespace KnightGame
