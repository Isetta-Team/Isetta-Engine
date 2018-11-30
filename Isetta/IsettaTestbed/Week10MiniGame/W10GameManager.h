/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "W10Player.h"
#include "W10NetworkPlayer.h"

BEGIN_COMPONENT(W10GameManager, Isetta::Component, true)
public:
W10Player* player;
W10NetworkPlayer* enemy;
END_COMPONENT(W10GameManager, Isetta::Component)
