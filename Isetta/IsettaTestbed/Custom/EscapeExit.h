/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Quit application on ESC key
 *
 */
namespace Isetta {
DEFINE_COMPONENT(EscapeExit, Component, false)
private:
U64 handle;

public:
void OnEnable() override;
void OnDisable() override;
DEFINE_COMPONENT_END(EscapeExit, Component)
}  // namespace Isetta