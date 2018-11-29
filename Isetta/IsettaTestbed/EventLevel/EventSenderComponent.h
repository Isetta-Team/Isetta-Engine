/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

BEGIN_COMPONENT(EventSenderComponent, Isetta::Component, true)
Isetta::U64 handleA{0}, handleB{0}, handleC{0};
void Start() override;
END_COMPONENT(EventSenderComponent, Isetta::Component)
