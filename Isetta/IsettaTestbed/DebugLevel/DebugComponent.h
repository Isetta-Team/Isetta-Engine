/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(DebugComponent, Component)
public:
void Update() override;
CREATE_COMPONENT_END(DebugComponent, Component)
}  // namespace Isetta