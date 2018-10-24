/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Audio/AudioSource.h"
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(Zombie, Component)
public:
void OnEnable() override;
void Update() override;
void TakeDamage(float damage);
static float speed;

private:
float health = 100;
bool isInitialized = false;
AudioSource audio;
CREATE_COMPONENT_END(Zombie, Component)
}  // namespace Isetta
