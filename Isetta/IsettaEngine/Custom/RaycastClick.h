/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(RaycastClick, Component)
public:
void OnEnable() override;
CREATE_COMPONENT_END(RaycastClick, Component)
}  // namespace Isetta
