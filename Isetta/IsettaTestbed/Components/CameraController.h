/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
class CameraController : public Component {
 public:
  void Update() override;
  void GuiUpdate() override;
private:
  float y = 9.8;
  float z = 4.6;
};
}  // namespace Isetta
