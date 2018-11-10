/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(GUIComponent, Component, false)
private:
static bool checkbox;

public:
void GuiUpdate() override;
END_COMPONENT(GUIComponent, Component)
}  // namespace Isetta
