/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

using namespace Isetta;

namespace KnightGame {
DEFINE_COMPONENT(SpinAttack, Component, true)
private:
const float axisMin = 0.6f;
float startAngle = 3.14f, lastAngle;
const float timeToComplete = 1.f;
float elapsedTime = 0;

bool started = false, extended = false;
int direction = 0;
const float anglePercision = 0.25f;

bool callbackInProgress;
Action<> callback;

public:
void Update() override;

void SetCallback(Action<> callback);
};  // namespace KnightGame
}  // namespace KnightGame

bool Isetta::ComponentRegistry<KnightGame::SpinAttack, Component,
                               true>::SpinAttackRegistered =
    Component::RegisterComponent(
        std::type_index(typeid(KnightGame::SpinAttack)),
        std::type_index(typeid(Component)), true);