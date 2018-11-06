/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "W10Player.h"

BEGIN_COMPONENT(W10GameManager, Isetta::Component, true)
public:
W10Player* player;
END_COMPONENT(W10GameManager, Isetta::Component)
