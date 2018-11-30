/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/DataStructures/Array.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(LevelLoadingMenu, Component, true)
private:
Array<std::string> levels;
static inline int handle = -1;
bool showWindow;
static LevelLoadingMenu* instance;

public:
LevelLoadingMenu();

void OnEnable() override;
void OnDisable() override;
void GuiUpdate() override;
DEFINE_COMPONENT_END(LevelLoadingMenu, Component)
}  // namespace Isetta
