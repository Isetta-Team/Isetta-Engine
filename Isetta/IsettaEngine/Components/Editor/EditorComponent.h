/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(EditorComponent, Component, true)
private:
class Console* console;
class Inspector* inspector;
class Hierarchy* hierarchy;
class FrameReporter* frameReporter;

int menuHandle, consoleHandle, inspectorHandle, hierarchyHandle,
    frameReporterHandle;
bool isOpen, menuOpen{true};

void OnDisable() override;
void GuiUpdate() override;

public:
EditorComponent() = default;

void Awake() override;
void OnEnable() override;
END_COMPONENT(EditorComponent, Component)
}  // namespace Isetta
