/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "W10Player.h"
#include "W10NetworkPlayer.h"

DEFINE_COMPONENT(W10GameManager, Isetta::Component, true)
public:
W10Player* player;
W10NetworkPlayer* enemy;
DEFINE_COMPONENT_END(W10GameManager, Isetta::Component)
