/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Core/IsettaAlias.h"

namespace Isetta {
DEFINE_COMPONENT(EditorComponent, Component, true)
private:
class Console* console;
class Inspector* inspector;
class Hierarchy* hierarchy;
class FrameReporter* frameReporter;

U64 menuHandle, consoleHandle, inspectorHandle, hierarchyHandle,
    frameReporterHandle;
bool isOpen, menuOpen{true};

void OnDisable() override;
void GuiUpdate() override;

public:
EditorComponent() = default;

void Awake() override;
void OnEnable() override;
DEFINE_COMPONENT_END(EditorComponent, Component)
}  // namespace Isetta
