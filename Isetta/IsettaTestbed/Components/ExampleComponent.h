/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(ExampleComponent, Component, true)
public : void OnEnable() override;
void Update() override;
CREATE_COMPONENT_END(ExampleComponent, Component)
}  // namespace Isetta
