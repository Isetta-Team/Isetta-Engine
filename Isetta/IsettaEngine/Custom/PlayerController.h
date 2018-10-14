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

private:
  float rotSpeed{75.f};
  float moveSpeed{5.0f};
};
}  // namespace Isetta
