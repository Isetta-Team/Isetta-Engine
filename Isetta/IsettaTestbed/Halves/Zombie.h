/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
namespace Isetta {
BEGIN_COMPONENT(Zombie, Component, true)
public:
void OnEnable() override;
void Update() override;
void TakeDamage(float damage);
static float speed;

private:
float health = 100;
bool isInitialized = false;
AudioSource* audio{nullptr};
END_COMPONENT(Zombie, Component)
}  // namespace Isetta
