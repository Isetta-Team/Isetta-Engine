/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
/**
 * @brief DebugDrawing example component with a number of the debug drawing
 * capabilities
 *
 */
namespace Isetta {
DEFINE_COMPONENT(DebugComponent, Component, false)
public:
void Update() override;
DEFINE_COMPONENT_END(DebugComponent, Component)
}  // namespace Isetta