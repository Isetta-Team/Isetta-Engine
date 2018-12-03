/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Color.h"
#include "Scene/Component.h"

namespace Isetta {
/**
 * @brief NOT DONE
 *  ----- IN DEVELOPMENT -----
 *
 */
BEGIN_COMPONENT(PauseComponent, Component, true)
private:
bool pause;
Color border = Color::blue;
int handle;

public:
void OnEnable() override;
void OnDisable() override;
void GuiUpdate() override;
void Update() override;

void PauseGame() { pause = !pause; }
END_COMPONENT(PauseComponent, Component)
}  // namespace Isetta