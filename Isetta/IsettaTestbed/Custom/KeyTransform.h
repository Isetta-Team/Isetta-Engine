/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
DEFINE_COMPONENT(KeyTransform, Component, true)
private:
  float step;
  bool pressed = true;

 public:
  KeyTransform(float step = 1) : step{step} {}

  void OnEnable() override;
  void Update() override;
DEFINE_COMPONENT_END(KeyTransform, Component)
}  // namespace Isetta
