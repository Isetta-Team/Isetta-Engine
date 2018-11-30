/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <vector>
namespace Isetta {
DEFINE_COMPONENT(PlayerController, Component, true)
public:
void OnEnable() override;
void Start() override;
void Update() override;
void GuiUpdate() override;
static PlayerController* Instance();

private:
static PlayerController* instance;
void Shoot();

float scale{0.01f};
float moveSpeed{15.0f};
bool isMoving{false};
float lookSmooth{30.f};

// shooting
std::vector<Entity*> bullets;
int bulletPoolSize = 100;
float shootInterval = 0.15f;
float cooldown{0};
AudioSource* shootAudio{nullptr};

AnimationComponent* animationComp{nullptr};

DEFINE_COMPONENT_END(PlayerController, Component)
}  // namespace Isetta
