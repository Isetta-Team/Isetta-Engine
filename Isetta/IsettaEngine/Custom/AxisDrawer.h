/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(AxisDrawer, Component)
public:
void Update() override;
CREATE_COMPONENT_END(AxisDrawer, Component)
}  // namespace Isetta
