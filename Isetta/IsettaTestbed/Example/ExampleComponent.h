/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(ExampleComponent, Component, true)
public : void OnEnable() override;
void Update() override;
END_COMPONENT(ExampleComponent, Component)
}  // namespace Isetta
