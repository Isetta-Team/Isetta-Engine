/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <vector>
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(GameManager, Component, true)
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
CREATE_COMPONENT_END(GameManager, Component)
}  // namespace Isetta
