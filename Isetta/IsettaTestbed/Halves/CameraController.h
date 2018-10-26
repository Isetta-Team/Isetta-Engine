/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(CameraController, Component, true)
public : void Update() override;
void GuiUpdate() override;

private:
float y = 9.8;
float z = 4.6;
CREATE_COMPONENT_END(CameraController, Component)
}  // namespace Isetta
