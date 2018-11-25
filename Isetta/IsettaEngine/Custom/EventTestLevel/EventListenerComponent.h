/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(EventListenerComponent, Component, true)
private:
int handle;

public:
void OnEnable() override;
void OnDisable() override;
END_COMPONENT(EventListenerComponent, Component)
}  // namespace Isetta
