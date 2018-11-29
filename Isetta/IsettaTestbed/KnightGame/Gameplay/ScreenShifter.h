/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

using namespace Isetta;

namespace Isetta {
class CollisionHandler;
}  // namespace Isetta

namespace KnightGame {
BEGIN_COMPONENT(ScreenShifter, Component, true)
private:
float speed = 0.8f;

public:
ScreenShifter() = default;

void Update() override;
};  // namespace KnightGame
}  // namespace KnightGame

using namespace KnightGame;
REGISTER_COMPONENT(ScreenShifter, Component, true)