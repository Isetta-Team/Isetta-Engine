/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
DEFINE_COMPONENT(CameraController, Component, true)
public : void Update() override;
void GuiUpdate() override;

private:
float y = 9.8;
float z = 4.6;
DEFINE_COMPONENT_END(CameraController, Component)
}  // namespace Isetta
