/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(Editor, Component, true)
private:
class Console* console;
class Inspector* inspector;
class Hierarchy* hierarchy;

int menuHandle, consoleHandle, inspectorHandle, hierarchyHandle;
bool isOpen, menuOpen{true};

void OnDisable() override;
void GuiUpdate() override;

public:
Editor() = default;

void Awake() override;
void OnEnable() override;
END_COMPONENT(Editor, Component)
}  // namespace Isetta
