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
BEGIN_COMPONENT(DebugComponent, Component, true)
public:
void Update() override;
END_COMPONENT(DebugComponent, Component)
}  // namespace Isetta