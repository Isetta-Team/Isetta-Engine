/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(KeyTransform, Component, true)
private:
  float step;
  bool pressed;

 public:
  KeyTransform(float step = 1) : step{step} {}

  void OnEnable() override;
  void Update() override;
END_COMPONENT(KeyTransform, Component)
}  // namespace Isetta
