/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

DEFINE_COMPONENT(W10Player, Isetta::Component, true)
public:
W10Player(bool isRight, int swordNetID, int clientAuthorityID);
void Awake() override;
void Start() override;
void Update() override;
Isetta::Entity* swordEntity;

private:
void InitPosition();
bool isOnRight;
bool canOperate;

float horizontalSpeed;

int swordPos;
float swordTargetX;
float swordXProgress;
float stabSpeed;
int swordStabStatus;  // 0 for init, 1 for atack, 2 for revoke, 3 for not on
                      // hand

float gravity;
float flyDuration;
float totalFlyDuration;
float targetX;
float targetY;
float originY;
float v0x;
float v0y;
bool isSwordFlying;
int clientAuthorityId;
int swordNetId;

void ChangeSwordVerticlePosition(int direction);
void ChangeSwordHorizontalPosition(float deltaTime);
void SwordBlocked();

DEFINE_COMPONENT_END(W10Player, Isetta::Component)
