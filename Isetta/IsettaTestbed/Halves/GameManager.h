/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <vector>
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(GameManager, Component, true)
public : void OnEnable() override;
void Update() override;
void GuiUpdate() override;

static std::vector<Entity*> zombies;
static int score;

private:
void SpawnZombie() const;
float spawnRadius{10};
float spawnInterval{2};
float cooldown{3};
int poolSize = 100;
END_COMPONENT(GameManager, Component)
}  // namespace Isetta
