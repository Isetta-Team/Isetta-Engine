/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkTransform.h"

#include <algorithm>
#include <unordered_map>
#include "Networking/NetworkId.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Scene/Level.h";
#include "Scene/LevelManager.h"

namespace Isetta {

bool NetworkTransform::registeredCallbacks = false;
std::unordered_map<int, float> NetworkTransform::posUpdateTimes;
std::unordered_map<int, float> NetworkTransform::rotUpdateTimes;
std::unordered_map<int, float> NetworkTransform::scaleUpdateTimes;

void NetworkTransform::Start() {
  if (!registeredCallbacks) {
    // Position callbacks
    NetworkManager::Instance().RegisterClientCallback<PositionMessage>(
        [&](yojimbo::Message* message) {
          PositionMessage* positionMessage =
              reinterpret_cast<PositionMessage*>(message);

          NetworkId* netId =
              NetworkManager::Instance().GetNetworkId(positionMessage->netId);
          if (!netId || netId->HasClientAuthority()) {
            return;
          }

          Entity* entity = netId->GetEntity();
          NetworkTransform* nt = entity->GetComponent<NetworkTransform>();

          if (nt && positionMessage->updateTime < nt->lastPosMessage) {
            return;
          }

          if (entity) {
            Transform* t = entity->GetTransform();
            nt->targetPos = positionMessage->localPos;

            nt->posInterpolation = 0;

            if ((Math::Vector3::Scale(t->GetWorldScale(),
                                      t->GetLocalPos() - nt->targetPos))
                    .SqrMagnitude() >= nt->snapDistance * nt->snapDistance) {
              t->SetLocalPos(positionMessage->localPos);
              nt->posInterpolation = 1;
            }

            nt->prevPos = t->GetLocalPos();
            nt->lastPosMessage = positionMessage->updateTime;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<PositionMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          PositionMessage* positionMessage =
              reinterpret_cast<PositionMessage*>(message);

          if (posUpdateTimes.find(positionMessage->netId) ==
              posUpdateTimes.end()) {
            posUpdateTimes[positionMessage->netId] =
                positionMessage->updateTime;
          } else if (posUpdateTimes[positionMessage->netId] <=
                     positionMessage->updateTime) {
            NetworkManager::Instance()
                .SendAllMessageFromServer<PositionMessage>(positionMessage);
          }
        });

    // Rotation callbacks
    NetworkManager::Instance().RegisterClientCallback<RotationMessage>(
        [&](yojimbo::Message* message) {
          RotationMessage* rotationMessage =
              reinterpret_cast<RotationMessage*>(message);

          NetworkId* netId =
              NetworkManager::Instance().GetNetworkId(rotationMessage->netId);
          if (!netId || netId->HasClientAuthority()) {
            return;
          }

          Entity* entity = netId->GetEntity();
          NetworkTransform* nt = entity->GetComponent<NetworkTransform>();

          if (nt && rotationMessage->updateTime < nt->lastRotMessage) {
            return;
          }

          if (entity) {
            Transform* t = entity->GetTransform();
            nt->targetRot = rotationMessage->localRot;

            nt->rotInterpolation = 0;

            if (abs(Math::Quaternion::AngleDeg(
                    t->GetLocalRot(), nt->targetRot)) >= nt->snapRotation) {
              t->SetLocalRot(rotationMessage->localRot);
              nt->rotInterpolation = 1;
            }

            nt->prevRot = t->GetLocalRot();
            nt->lastRotMessage = rotationMessage->updateTime;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<RotationMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          RotationMessage* rotationMessage =
              reinterpret_cast<RotationMessage*>(message);

          if (rotUpdateTimes.find(rotationMessage->netId) ==
              rotUpdateTimes.end()) {
            rotUpdateTimes[rotationMessage->netId] =
                rotationMessage->updateTime;
          } else if (rotUpdateTimes[rotationMessage->netId] <=
                     rotationMessage->updateTime) {
            NetworkManager::Instance()
                .SendAllMessageFromServer<RotationMessage>(rotationMessage);
          }
        });

    // Scale callbacks
    NetworkManager::Instance().RegisterClientCallback<ScaleMessage>(
        [&](yojimbo::Message* message) {
          ScaleMessage* scaleMessage = reinterpret_cast<ScaleMessage*>(message);

          NetworkId* netId =
              NetworkManager::Instance().GetNetworkId(scaleMessage->netId);
          if (!netId || netId->HasClientAuthority()) {
            return;
          }

          Entity* entity = netId->GetEntity();
          NetworkTransform* nt = entity->GetComponent<NetworkTransform>();

          if (nt && scaleMessage->updateTime < nt->lastScaleMessage) {
            return;
          }

          if (entity) {
            Transform* t = entity->GetTransform();
            nt->targetScale = scaleMessage->localScale;

            nt->scaleInterpolation = 0;

            if ((t->GetLocalScale() - nt->targetScale).SqrMagnitude() >=
                nt->snapScale * nt->snapScale) {
              t->SetLocalScale(scaleMessage->localScale);
              nt->scaleInterpolation = 1;
            }

            nt->prevScale = t->GetLocalScale();
            nt->lastScaleMessage = scaleMessage->updateTime;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<ScaleMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          ScaleMessage* scaleMessage = reinterpret_cast<ScaleMessage*>(message);

          if (scaleUpdateTimes.find(scaleMessage->netId) ==
              scaleUpdateTimes.end()) {
            scaleUpdateTimes[scaleMessage->netId] = scaleMessage->updateTime;
          } else if (scaleUpdateTimes[scaleMessage->netId] <=
                     scaleMessage->updateTime) {
            NetworkManager::Instance().SendAllMessageFromServer<ScaleMessage>(
                scaleMessage);
          }
        });

    NetworkTransform::registeredCallbacks = true;
  }
  netId = entity->GetComponent<NetworkId>();
  targetPos = entity->GetTransform()->GetLocalPos();
  prevPos = targetPos;
  targetRot = entity->GetTransform()->GetLocalRot();
  prevRot = targetRot;
  targetScale = entity->GetTransform()->GetLocalScale();
  prevScale = targetScale;

  lastPosMessage = 0;
  lastRotMessage = 0;
  lastScaleMessage = 0;
}

void NetworkTransform::FixedUpdate() {
  if (netId->HasClientAuthority()) {
    ++updateCounter;

    if (updateCounter >= netId->updateInterval) {
      updateCounter = 0;

      Transform* t = entity->GetTransform();
      // Position
      if (Math::Vector3::Scale(t->GetParent()->GetWorldScale(),
                               t->GetLocalPos() - prevPos)
              .SqrMagnitude() >= updateDistance * updateDistance) {
        PositionMessage* message =
            NetworkManager::Instance()
                .GenerateMessageFromClient<PositionMessage>();
        prevPos = t->GetLocalPos();
        message->localPos = t->GetLocalPos();
        message->netId = netId->id;
        NetworkManager::Instance().SendMessageFromClient(message);
      }
      // Rotation
      if (Math::Quaternion::AngleDeg(t->GetLocalRot(), prevRot) >=
          updateRotation) {
        RotationMessage* message =
            NetworkManager::Instance()
                .GenerateMessageFromClient<RotationMessage>();
        prevRot = t->GetLocalRot();
        message->localRot = t->GetLocalRot();
        message->netId = netId->id;
        NetworkManager::Instance().SendMessageFromClient(message);
      }
      // Scale
      if ((t->GetLocalScale() - prevScale).SqrMagnitude() >=
          updateScale * updateScale) {
        ScaleMessage* message = NetworkManager::Instance()
                                    .GenerateMessageFromClient<ScaleMessage>();
        prevScale = t->GetLocalScale();
        message->localScale = t->GetLocalScale();
        message->netId = netId->id;
        NetworkManager::Instance().SendMessageFromClient(message);
      }
    }
  } else {
    Transform* t = entity->GetTransform();
    float netIdLerp = 1.0 / netId->updateInterval;

    // Translation
    posInterpolation = min(posInterpolation + netIdLerp, 1);
    t->SetLocalPos(Math::Vector3::Lerp(prevPos, targetPos, posInterpolation));
    // Rotation
    rotInterpolation = min(rotInterpolation + netIdLerp, 1);
    t->SetLocalRot(
        Math::Quaternion::Lerp(prevRot, targetRot, rotInterpolation));
    // Scale
    scaleInterpolation = min(scaleInterpolation + netIdLerp, 1);
    t->SetLocalScale(
        Math::Vector3::Lerp(prevScale, targetScale, scaleInterpolation));
  }
}

void NetworkTransform::ForceSendTransform() {
  Transform* t = entity->GetTransform();
  // Position
  PositionMessage* pMessage =
      NetworkManager::Instance().GenerateMessageFromClient<PositionMessage>();
  prevPos = t->GetLocalPos();
  pMessage->localPos = t->GetLocalPos();
  pMessage->netId = netId->id;
  NetworkManager::Instance().SendMessageFromClient(pMessage);

  // Rotation
  RotationMessage* rMessage =
      NetworkManager::Instance().GenerateMessageFromClient<RotationMessage>();
  prevRot = t->GetLocalRot();
  rMessage->localRot = t->GetLocalRot();
  rMessage->netId = netId->id;
  NetworkManager::Instance().SendMessageFromClient(rMessage);

  // Scale
  ScaleMessage* sMessage =
      NetworkManager::Instance().GenerateMessageFromClient<ScaleMessage>();
  prevScale = t->GetLocalScale();
  sMessage->localScale = t->GetLocalScale();
  sMessage->netId = netId->id;
  NetworkManager::Instance().SendMessageFromClient(sMessage);
}

void NetworkTransform::SnapTransform() {
  Transform* t = entity->GetTransform();
  prevPos = targetPos;
  prevRot = targetRot;
  prevScale = targetScale;
  t->SetLocalPos(targetPos);
  t->SetLocalRot(targetRot);
  t->SetLocalScale(targetScale);
}

void NetworkTransform::SetNetworkedParentToRoot() {
  entity->GetTransform()->SetParent(nullptr);
}

bool NetworkTransform::SetNetworkedParent(int networkId) {
  Entity* parent = NetworkManager::Instance().GetNetworkEntity(networkId);
  if (!parent) {
    return false;
  }

  entity->GetTransform()->SetParent(parent->GetTransform());
  return true;
}
}  // namespace Isetta
