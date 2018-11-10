/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector2.h"
#include "ISETTA_API.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(FlyController, Component, false)
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
  float flyMultiplier = 2.0f;
  bool enableLook = true;
END_COMPONENT(FlyController, Component)
}  // namespace Isetta
