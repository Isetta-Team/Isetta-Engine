/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(KeyTransform, Component, true)
private:
  float step;
  bool pressed;

 public:
  KeyTransform(float step = 1) : step{step} {}

  void OnEnable() override;
  void Update() override;
CREATE_COMPONENT_END(KeyTransform, Component)
}  // namespace Isetta
