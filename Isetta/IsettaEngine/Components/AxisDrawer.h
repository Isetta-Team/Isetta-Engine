/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(AxisDrawer, Component, true)
public:
void Update() override;
DEFINE_COMPONENT_END(AxisDrawer, Component)
}  // namespace Isetta
