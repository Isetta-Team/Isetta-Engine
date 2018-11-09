/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Scene/Entity.h"

BEGIN_COMPONENT(W10NetworkPlayer, Isetta::Component, true)
Isetta::Entity* swordEntity;
int swordNetId;
bool isOnRight;
int clientAuthorityId;
public:
W10NetworkPlayer(bool isRight, int swordNetID, int clientAuthorityID);
void Awake() override;
void SwordBlocked();
void SwordCollected();
void Update() override;
END_COMPONENT(W10NetworkPlayer, Isetta::Component)
