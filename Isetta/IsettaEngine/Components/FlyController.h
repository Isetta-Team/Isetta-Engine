/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector2.h"
#include "ISETTA_API.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(FlyController, Component, false)
public:
  FlyController() = default;
  FlyController(bool inControl);

  void OnEnable() override;
  void Update() override;

 private:
  Math::Vector2 lastFrameMousePos;
  float rotX{};
  float rotY{};
  float lookRotationSpeed{-0.1f};
  float flySpeed = 15.f;
  float flySpeedMultiplier = 2.0f;
  bool enableLook = true;
DEFINE_COMPONENT_END(FlyController, Component)
}  // namespace Isetta
