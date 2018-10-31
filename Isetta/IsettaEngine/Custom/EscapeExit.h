/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(EscapeExit, Component, true)
private:
int handle;

public:
void OnEnable() override;
void OnDisable() override;
CREATE_COMPONENT_END(EscapeExit, Component)
}  // namespace Isetta