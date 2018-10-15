/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Audio/AudioSource.h"

namespace Isetta {
class PlayerController : public Component {
 public:
  void OnEnable() override;
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
};
}  // namespace Isetta
