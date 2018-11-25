/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

using namespace Isetta;

namespace Isetta {
class Collider;
class CollisionHandler;
class AudioClip;
class AudioSource;
}  // namespace Isetta

namespace KnightGame {
BEGIN_COMPONENT(Enemy, Component, true)
private:
CollisionHandler* handler;
int handle;
AudioSource* src;
AudioClip* const swordClip;

public:
Enemy() = default;
Enemy(CollisionHandler* handler, AudioClip* const swordClip)
    : handler{handler}, swordClip{swordClip} {}

void OnEnable() override;
void OnDisable() override;
void OnCollisionEnter(Collider* const collider);

void Explode();
};  // namespace KnightGame
}  // namespace KnightGame

using namespace KnightGame;
REGISTER_COMPONENT(Enemy, Component, true)