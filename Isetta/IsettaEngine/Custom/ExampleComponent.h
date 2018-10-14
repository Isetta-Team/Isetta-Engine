/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
class ISETTA_API ExampleComponent : public Component {
 public:
  void OnEnable() override;
  void Update() override;
};
}  // namespace Isetta
