/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(GameMgtTestComp, Component, true)
public:
void Start() override;
void GuiUpdate() override;
END_COMPONENT(GameMgtTestComp, Component)
}  // namespace Isetta