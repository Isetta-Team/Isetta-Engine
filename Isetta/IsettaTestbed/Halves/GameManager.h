/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <vector>

namespace Isetta {
DEFINE_COMPONENT(GameManager, Component, true)
public:
void Start() override;
void OnEnable() override;
void Update() override;
void GuiUpdate() override;

static std::vector<Entity*> zombies;
static int score;

private:
class Font* font;
void SpawnZombie() const;
float spawnRadius{10};
float spawnInterval{2};
float cooldown{3};
int poolSize = 100;
DEFINE_COMPONENT_END(GameManager, Component)
}  // namespace Isetta
