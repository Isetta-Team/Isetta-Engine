/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
class KeyTransform : public Component {
 private:
  float step;

 public:
  KeyTransform(float step = 1) : step{step} {}

  void OnEnable() override;
};
}  // namespace Isetta
