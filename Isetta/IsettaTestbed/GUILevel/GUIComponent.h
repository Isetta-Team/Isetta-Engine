/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Some of the GUI functionality
 *
 */
namespace Isetta {
BEGIN_COMPONENT(GUIComponent, Component, false)
private:
static bool checkbox;

public:
void GuiUpdate() override;
END_COMPONENT(GUIComponent, Component)
}  // namespace Isetta
