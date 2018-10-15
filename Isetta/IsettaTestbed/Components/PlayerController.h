/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
class PlayerController : public Component {
 public:
  void OnEnable() override;
  void Update() override;
  void GuiUpdate() override;
  static PlayerController* Instance();

 private:
  static PlayerController* instance;
  float moveSpeed{15.0f};
  int moveSpeedInt{15};
  float lookSmooth{10.f};
  int scaleInt{1};
};
}  // namespace Isetta
