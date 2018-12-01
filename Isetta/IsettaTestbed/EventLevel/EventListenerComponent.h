/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Listen for event and output message
 *
 */
namespace Isetta {
DEFINE_COMPONENT(EventListenerComponent, Component, false)
private:
int handle;

public:
void OnEnable() override;
void OnDisable() override;
DEFINE_COMPONENT_END(EventListenerComponent, Component)
}  // namespace Isetta
