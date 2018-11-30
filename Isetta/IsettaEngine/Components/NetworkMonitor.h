/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(NetworkMonitor, Component, true)
public:
void GuiUpdate() override;
DEFINE_COMPONENT_END(NetworkMonitor, Component)
}  // namespace Isetta