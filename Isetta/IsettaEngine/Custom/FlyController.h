/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "ISETTA_API.h"
#include "Scene/Component.h"

namespace Isetta {
class ISETTA_API FlyController : public Component {
 public:
  void OnEnable() override;
  void Update() override;
  void GuiUpdate() override;
};
}  // namespace Isetta
