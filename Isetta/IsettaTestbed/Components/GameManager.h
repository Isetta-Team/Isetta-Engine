/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include <vector>

namespace Isetta {
class GameManager : public Component {
 public:
  void OnEnable() override;
  void Update() override;
  void GuiUpdate() override;

  static std::vector<Entity*> zombies;
  static int score;
private:
  void SpawnZombie() const;
  float spawnRadius{10};
  float spawnInterval {2};
  float cooldown{3};
  int poolSize = 100;
};
}  // namespace Isetta
