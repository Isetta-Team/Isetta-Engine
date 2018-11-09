/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

BEGIN_COMPONENT(W10UIManager, Isetta::Component, true)
public:
void Awake() override;
void GuiUpdate() override;
std::string displayText = "Press R to ready.";
END_COMPONENT(W10UIManager, Isetta::Component)
