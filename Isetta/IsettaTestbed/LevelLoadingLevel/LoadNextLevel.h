/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(LoadNextLevel, Component, true)
private:
std::string loadLevel;
int handle;

public:
LoadNextLevel(std::string_view loadLevel) : loadLevel{loadLevel} {}

void OnEnable() override;
void OnDisable() override;
END_COMPONENT(LoadNextLevel, Component)
}  // namespace Isetta
