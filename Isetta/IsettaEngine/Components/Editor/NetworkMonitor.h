/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
/**
 * @brief Network monitor interface window for displaying useful networking
 * information
 *
 */
DEFINE_COMPONENT(NetworkMonitor, Component, false)
#ifdef _EDITOR
public:
void GuiUpdate() override;
#endif
DEFINE_COMPONENT_END(NetworkMonitor, Component)
}  // namespace Isetta