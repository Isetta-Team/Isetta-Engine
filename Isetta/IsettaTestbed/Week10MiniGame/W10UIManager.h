/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

DEFINE_COMPONENT(W10UIManager, Isetta::Component, true)
int winning;
int losing;
public:
void Awake() override;
void GuiUpdate() override;
std::string displayText = "Press R to ready.";
DEFINE_COMPONENT_END(W10UIManager, Isetta::Component)
