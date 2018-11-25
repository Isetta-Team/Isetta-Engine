/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(NoCameraComponent, Component, true)
void GuiUpdate() override;
END_COMPONENT(NoCameraComponent, Component)
}  // namespace Isetta
