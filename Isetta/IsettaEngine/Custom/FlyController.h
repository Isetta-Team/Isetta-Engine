/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "ISETTA_API.h"
#include "Scene/Component.h"
#include "Core/Math/Vector2.h"

namespace Isetta {
class ISETTA_API FlyController : public Component {
 public:
  void OnEnable() override;
  void Update() override;
  void GuiUpdate() override;

private:
  Math::Vector2 lastFrameMousePos;
  float rotX{};
  float rotY{};
  float lookRotationSpeed{-0.1f};
  float flySpeed = 15.f;
};
}  // namespace Isetta
