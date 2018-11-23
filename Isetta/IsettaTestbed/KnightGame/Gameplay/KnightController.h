/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

using namespace Isetta;

namespace Isetta {
class AnimationComponent;
}

namespace KnightGame {
BEGIN_COMPONENT(KnightController, Component, true)
public:
void Start() override;
void Update() override;

private:
AnimationComponent* animation;
int death, idle, impact, run, slash, spin, walk;

const float walkSpeed = 0.1f, runSpeed = 1;
const float walkAxis = 0.4f, runAxis = 0.7f;
const float depth = 4.5f;

};  // namespace KnightGame
}  // namespace KnightGame

bool Isetta::ComponentRegistry<KnightGame::KnightController, Component,
                               true>::KnightControllerRegistered =
    Component::RegisterComponent(
        std::type_index(typeid(KnightGame::KnightController)),
        std::type_index(typeid(Component)), true);