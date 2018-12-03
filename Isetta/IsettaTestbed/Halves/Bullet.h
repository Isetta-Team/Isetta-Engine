/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
DEFINE_COMPONENT(Bullet, Component, true)
public:
void Reactivate(const Math::Vector3& pos, const Math::Vector3& flyDir);
void OnEnable() override;
void Update() override;

// exposed for easier debug inspector
static float flySpeed;

private:
float lifeTime{1.5};
float elapsedTime{0};
float damage{35};
Math::Vector3 dir{};
bool initialized = false;
AudioSource* audio;
DEFINE_COMPONENT_END(Bullet, Component)
}  // namespace Isetta
