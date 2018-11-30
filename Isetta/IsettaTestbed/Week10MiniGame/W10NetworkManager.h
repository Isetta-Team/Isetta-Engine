/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Week10MiniGame/W10GameManager.h"
#include "Core/DataStructures/HandleBin.h"

DEFINE_COMPONENT(W10NetworkManager, Isetta::Component, true)
void HandleReadyMessage(int clientIdx, yojimbo::Message* message);
void HandleSpawnMessage(yojimbo::Message* message);
void HandleSwordPosMessage(int clientIdx, yojimbo::Message* message);
void ClientHandleAttackAttemptMessage(yojimbo::Message* message);
void ServerHandleAttackAttemptMessage(int clientIdx, yojimbo::Message* message);
void HandlePositionReport(int clientIdx, yojimbo::Message* message);
void HandleAttackResultMessage(yojimbo::Message* message);
Isetta::U16 readyHandle;
Isetta::U16 spawnHandle;
Isetta::U16 swordHandle;
Isetta::U16 clientAttackHandle;
Isetta::U16 serverAttackHandle;
Isetta::U16 positionHandle;
Isetta::U16 resultHandle;

// std::vector<int> clientIDs;

float killDistance = 0.825;
float blockDistance = 1.125;

Isetta::HandleBin netIdBin{1};

int clientCount;
int lastAttemptClient;
std::unordered_map<int, float> clientPosX;
std::unordered_map<int, int> clientSwordPos;
W10GameManager* gameManager;

public:
void Awake() override;
void Start() override;
DEFINE_COMPONENT_END(W10NetworkManager, Isetta::Component)

DEFINE_NETWORK_MESSAGE(W10ReadyMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {}
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(W10SpawnMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);
  serialize_int(stream, clientAuthorityId, 0,
                Isetta::NetworkManager::Instance().GetMaxClients());
  serialize_int(stream, swordNetId, 0, 256);
  serialize_bool(stream, isOnRight);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const W10SpawnMessage* message =
      reinterpret_cast<const W10SpawnMessage*>(otherMessage);
  netId = message->netId;
  clientAuthorityId = message->clientAuthorityId;
  isOnRight = message->isOnRight;
  swordNetId = message->swordNetId;
}

int netId = 0, clientAuthorityId = 0, swordNetId = 0;
bool isOnRight;
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(W10SwordPosMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, swordPos, -1, 1);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const W10SwordPosMessage* message =
      reinterpret_cast<const W10SwordPosMessage*>(otherMessage);
  swordPos = message->swordPos;
}

int swordPos = 0;
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(W10AttackAttemptMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {}
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(W10PositionReportMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_float(stream, positionX);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const W10PositionReportMessage* message =
      reinterpret_cast<const W10PositionReportMessage*>(otherMessage);
  positionX = message->positionX;
}

float positionX;
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(W10AttackResultMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, result, 0, 3);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const W10AttackResultMessage* message =
      reinterpret_cast<const W10AttackResultMessage*>(otherMessage);
  result = message->result;
}

int result = 0;  // 0 - Win, 1 - Lose, 2 - Blocked, 3 - Block Other
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(W10CollectMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, swordNetId, 0, 256);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const W10CollectMessage* message =
      reinterpret_cast<const W10CollectMessage*>(otherMessage);
  swordNetId = message->swordNetId;
}

int swordNetId;
DEFINE_NETWORK_MESSAGE_END
