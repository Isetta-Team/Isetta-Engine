/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector2.h"
#include "Scene/Component.h"

using namespace Isetta;

namespace KnightGame {
BEGIN_COMPONENT(SpinAttack, Component, true)
private:
const float axisMin = 0.6f;
float startAngle = 3.14f, lastAngle;
const float timeToComplete = 1.f;
float elapsedTime = 0;

bool started = false, extended = false;
int direction = 0;
const float anglePercision = 0.25f;

public:
void Update() override;
};  // namespace KnightGame
}  // namespace KnightGame

bool Isetta::ComponentRegistry<KnightGame::SpinAttack, Component,
                               true>::SpinAttackRegistered =
    Component::RegisterComponent(
        std::type_index(typeid(KnightGame::SpinAttack)),
        std::type_index(typeid(Component)), true);