/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
class FlyController : public Component {
 public:
  void OnEnable() override;
  void Update() override;
  void GuiUpdate() override;
};
}  // namespace Isetta
