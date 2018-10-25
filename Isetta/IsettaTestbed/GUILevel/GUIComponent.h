/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(GUIComponent, Component)
private:
static bool checkbox;

public:
void GuiUpdate() override;
CREATE_COMPONENT_END(GUIComponent, Component)
}  // namespace Isetta