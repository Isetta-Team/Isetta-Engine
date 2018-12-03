/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Loads the next level specified in constructor
 *
 */
namespace Isetta {
DEFINE_COMPONENT(LoadNextLevel, Component, true)
private:
std::string loadLevel;

public:
LoadNextLevel(std::string_view loadLevel) : loadLevel{loadLevel} {}

void GuiUpdate() override;
DEFINE_COMPONENT_END(LoadNextLevel, Component)
}  // namespace Isetta
