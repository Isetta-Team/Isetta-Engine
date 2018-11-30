/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Some of the GUI functionality
 *
 */
namespace Isetta {
DEFINE_COMPONENT(GUIComponent, Component, false)
private:
static bool checkbox;

public:
void GuiUpdate() override;
DEFINE_COMPONENT_END(GUIComponent, Component)
}  // namespace Isetta
