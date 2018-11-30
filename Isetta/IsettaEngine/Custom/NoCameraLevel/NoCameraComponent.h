/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(NoCameraComponent, Component, true)
void GuiUpdate() override;
DEFINE_COMPONENT_END(NoCameraComponent, Component)
}  // namespace Isetta
