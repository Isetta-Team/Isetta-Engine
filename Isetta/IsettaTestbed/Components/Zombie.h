/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Audio/AudioSource.h"

namespace Isetta {
class Zombie : public Component {
 public:
  void OnEnable() override;
  void Update() override;
  void TakeDamage(float damage);
  static float speed;
private:
  float health = 100;
  bool isInitialized = false;
  AudioSource audio;
};
}  // namespace Isetta
