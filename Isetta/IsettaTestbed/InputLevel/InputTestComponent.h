/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

CREATE_COMPONENT_BEGIN(InputTestComponent, Isetta::Component, false)
void Start() override;
void Update() override;
CREATE_COMPONENT_END(InputTestComponent, Isetta::Component)
