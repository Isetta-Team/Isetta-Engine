/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
/**
 * @brief Draws local axis which are hidden without _EDITOR
 *
 */
DEFINE_COMPONENT(AxisDrawer, Component, true)
public:
#ifdef _EDITOR
void Update() override;
#endif
DEFINE_COMPONENT_END(AxisDrawer, Component)
}  // namespace Isetta
