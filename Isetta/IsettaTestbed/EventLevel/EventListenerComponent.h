/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
BEGIN_COMPONENT(EventListenerComponent, Component, true)
private:
int handle;

public:
void OnEnable() override;
void OnDisable() override;
END_COMPONENT(EventListenerComponent, Component)
}  // namespace Isetta
