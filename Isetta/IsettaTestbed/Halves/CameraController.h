/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
BEGIN_COMPONENT(CameraController, Component, true)
public : void Update() override;
void GuiUpdate() override;

private:
float y = 9.8;
float z = 4.6;
END_COMPONENT(CameraController, Component)
}  // namespace Isetta
