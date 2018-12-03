/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
/**
 * @brief EditorComponent contains other editor windows including: Console,
 * FrameReporter, Hierarchy, and Inspector
 *  ALT + SHIFT + M: Toggles open menu bar (can also open the other windows)
 *  ALT + SHIFT + C: Toggles open console
 *  ALT + SHIFT + H: Toggles open hierarchy
 *  ALT + SHIFT + I: Toggles open inspector
 *  ALT + SHIFT + F: Toggles open frame reporter
 *
 */
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
