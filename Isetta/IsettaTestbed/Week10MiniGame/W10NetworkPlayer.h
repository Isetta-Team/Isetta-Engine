/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
DEFINE_COMPONENT(W10NetworkPlayer, Isetta::Component, true)
int swordNetId;
bool isOnRight;
int clientAuthorityId;
void InitPosition();

public:
W10NetworkPlayer(bool isRight, int swordNetID, int clientAuthorityID);
void Awake() override;
void SwordBlocked();
void Update() override;
Isetta::Entity* swordEntity;

DEFINE_COMPONENT_END(W10NetworkPlayer, Isetta::Component)
