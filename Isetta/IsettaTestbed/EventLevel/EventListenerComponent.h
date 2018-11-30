/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
DEFINE_COMPONENT(EventListenerComponent, Component, true)
private:
int handle;

public:
void OnEnable() override;
void OnDisable() override;
DEFINE_COMPONENT_END(EventListenerComponent, Component)
}  // namespace Isetta
