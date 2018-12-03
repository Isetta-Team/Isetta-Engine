/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

using namespace Isetta;

namespace Isetta {
class ParticleSystemComponent;
class Transform;
class AudioSource;
}  // namespace Isetta

namespace KnightGame {
DEFINE_COMPONENT(FireballCircle, Component, true)
private:
float elapsedTime = 0, timeUp = 3.f;
ParticleSystemComponent* particleSys = nullptr;
float radius = 2.f;
float angle = 0;
float speed = 30.f;
Transform* center;
class SpinAttack* spin;
AudioSource* src;

public:
std::list<class Enemy*> enemies;

FireballCircle() = default;
FireballCircle(Transform* center, SpinAttack* spin)
    : center{center}, spin{spin} {}

void Start() override;
void OnEnable() override;
void OnDisable() override;
void Update() override;
};  // namespace KnightGame
}  // namespace KnightGame

using namespace KnightGame;
REGISTER_COMPONENT(FireballCircle, Component, true)
