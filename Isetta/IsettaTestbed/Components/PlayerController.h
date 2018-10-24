/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <vector>
#include "Audio/AudioSource.h"
#include "Graphics/AnimationComponent.h"
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(PlayerController, Component)
public : void OnEnable() override;
void Start() override;
void Update() override;
void GuiUpdate() override;
static PlayerController* Instance();

private:
static PlayerController* instance;
void Shoot();
float moveSpeed{15.0f};
float lookSmooth{30.f};
float scale{0.01f};
// shooting
float shootInterval = 0.15f;
float cooldown{0};
AudioSource shootAudio;
int poolSize = 100;
AnimationComponent* animationComp;
bool isMoving;
std::vector<Entity*> bullets;
CREATE_COMPONENT_END(PlayerController, Component)
}  // namespace Isetta
