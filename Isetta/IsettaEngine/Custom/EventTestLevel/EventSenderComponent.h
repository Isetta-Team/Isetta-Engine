/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

CREATE_COMPONENT_BEGIN(EventSenderComponent, Isetta::Component, true)
void Start() override;
CREATE_COMPONENT_END(EventSenderComponent, Isetta::Component)
