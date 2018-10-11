/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "ISETTA_API.h"
#include "Scene/Component.h"
#include "Core/Math/Vector2.h"

namespace Isetta {
ISETTA_API class FlyController : public Component {
 public:
  void OnEnable() override;
  void Update() override;
  void GuiUpdate() override;

private:
  Math::Vector2 lastFrameMousePos;
};
}  // namespace Isetta
