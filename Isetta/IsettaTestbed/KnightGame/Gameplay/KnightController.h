/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

using namespace Isetta;

namespace Isetta {
class AnimationComponent;
}

namespace KnightGame {
DEFINE_COMPONENT(KnightController, Component, true)
public:
void Start() override;
void Update() override;

void Walk() { canRun = false; }
void Run() { canRun = true; }

private:
AnimationComponent* animation;
int death, idle, impact, run, slash, spin, walk;

const float walkSpeed = 0.35f, runSpeed = 1.1f;
const float walkAxis = 0.4f, runAxis = 0.7f;
const float depth = 4.5f;

bool canRun = true;

};  // namespace KnightGame
}  // namespace KnightGame

using namespace KnightGame;
REGISTER_COMPONENT(KnightController, Component, true)