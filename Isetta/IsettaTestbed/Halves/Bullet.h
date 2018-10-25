/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Audio/AudioSource.h"
#include "Core/Math/Vector3.h"
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(Bullet, Component, true)
public:
void Initialize(const Math::Vector3& pos, const Math::Vector3& flyDir);
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
AudioSource audio;
CREATE_COMPONENT_END(Bullet, Component)
}  // namespace Isetta
