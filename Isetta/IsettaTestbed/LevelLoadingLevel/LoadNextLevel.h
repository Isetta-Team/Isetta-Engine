/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

/**
 * @brief Loads the next level specified in constructor
 *
 */
namespace Isetta {
BEGIN_COMPONENT(LoadNextLevel, Component, true)
private:
std::string loadLevel;

public:
LoadNextLevel(std::string_view loadLevel) : loadLevel{loadLevel} {}

void GuiUpdate() override;
END_COMPONENT(LoadNextLevel, Component)
}  // namespace Isetta
