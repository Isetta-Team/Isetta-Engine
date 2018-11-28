/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(NetworkMonitor, Component, true)
public:
void GuiUpdate() override;
END_COMPONENT(NetworkMonitor, Component)
}  // namespace Isetta