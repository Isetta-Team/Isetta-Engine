/*
 * Copyright (c) 2018 Isetta
 */

#include "Components/NetworkTransform.h"

#include <algorithm>
#include <unordered_map>
#include "Networking/NetworkId.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace Isetta {

bool NetworkTransform::registeredCallbacks = false;
std::unordered_map<int, float> NetworkTransform::serverPosTimestamps;
std::unordered_map<int, float> NetworkTransform::serverRotTimestamps;
std::unordered_map<int, float> NetworkTransform::serverScaleTimestamps;

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

          if (nt && positionMessage->timestamp < nt->lastPosMessage) {
            return;
          }

          if (entity) {
            Transform* t = entity->GetTransform();
            nt->targetPos = positionMessage->worldPos;

            nt->posInterpolation = 0;

            if ((t->GetWorldPos() - nt->targetPos).SqrMagnitude() >=
                nt->snapDistance * nt->snapDistance) {
              t->SetWorldPos(positionMessage->worldPos);
              nt->posInterpolation = 1;
            }

            nt->prevPos = t->GetWorldPos();
            nt->lastPosMessage = positionMessage->timestamp;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<PositionMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          PositionMessage* positionMessage =
              reinterpret_cast<PositionMessage*>(message);

          if (serverPosTimestamps[positionMessage->netId] <=
              positionMessage->timestamp) {
            serverPosTimestamps[positionMessage->netId] =
                positionMessage->timestamp;
            NetworkManager::Instance()
                .SendAllButClientMessageFromServer<PositionMessage>(
                    clientIdx, positionMessage);
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

          if (nt && rotationMessage->timestamp < nt->lastRotMessage) {
            return;
          }

          if (entity) {
            Transform* t = entity->GetTransform();
            nt->targetRot = rotationMessage->worldRot;

            nt->rotInterpolation = 0;

            if (abs(Math::Quaternion::AngleDeg(
                    t->GetWorldRot(), nt->targetRot)) >= nt->snapRotation) {
              t->SetWorldRot(rotationMessage->worldRot);
              nt->rotInterpolation = 1;
            }

            nt->prevRot = t->GetWorldRot();
            nt->lastRotMessage = rotationMessage->timestamp;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<RotationMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          RotationMessage* rotationMessage =
              reinterpret_cast<RotationMessage*>(message);

          if (serverRotTimestamps[rotationMessage->netId] <=
              rotationMessage->timestamp) {
            serverRotTimestamps[rotationMessage->netId] =
                rotationMessage->timestamp;
            NetworkManager::Instance()
                .SendAllButClientMessageFromServer<RotationMessage>(
                    clientIdx, rotationMessage);
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

          if (nt && scaleMessage->timestamp < nt->lastScaleMessage) {
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
            nt->lastScaleMessage = scaleMessage->timestamp;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<ScaleMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          ScaleMessage* scaleMessage = reinterpret_cast<ScaleMessage*>(message);

          if (serverScaleTimestamps[scaleMessage->netId] <=
              scaleMessage->timestamp) {
            serverScaleTimestamps[scaleMessage->netId] =
                scaleMessage->timestamp;
            NetworkManager::Instance()
                .SendAllButClientMessageFromServer<ScaleMessage>(clientIdx,
                                                                 scaleMessage);
          }
        });

    NetworkTransform::registeredCallbacks = true;
  }
  netId = entity->GetComponent<NetworkId>();
  targetPos = entity->GetTransform()->GetWorldPos();
  prevPos = targetPos;
  targetRot = entity->GetTransform()->GetWorldRot();
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
      if ((t->GetWorldPos() - prevPos).SqrMagnitude() >=
          updateDistance * updateDistance) {
        PositionMessage* message =
            NetworkManager::Instance()
                .GenerateMessageFromClient<PositionMessage>();
        prevPos = t->GetWorldPos();
        message->worldPos = t->GetWorldPos();
        message->netId = netId->id;
        NetworkManager::Instance().SendMessageFromClient(message);
      }
      // Rotation
      if (Math::Quaternion::AngleDeg(t->GetWorldRot(), prevRot) >=
          updateRotation) {
        RotationMessage* message =
            NetworkManager::Instance()
                .GenerateMessageFromClient<RotationMessage>();
        prevRot = t->GetWorldRot();
        message->worldRot = t->GetWorldRot();
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
  } else if (posInterpolation < 1 || rotInterpolation < 1 || scaleInterpolation < 1) {
    Transform* t = entity->GetTransform();
    float netIdLerp = netId->interpolationFactor / netId->updateInterval;

    // Translation
    posInterpolation = min(posInterpolation + netIdLerp, 1);
    t->SetWorldPos(Math::Vector3::Lerp(prevPos, targetPos, posInterpolation));
    // Rotation
    rotInterpolation = min(rotInterpolation + netIdLerp, 1);
    t->SetWorldRot(
        Math::Quaternion::Slerp(prevRot, targetRot, rotInterpolation));
    // Scale
    scaleInterpolation = min(scaleInterpolation + netIdLerp, 1);
    t->SetLocalScale(
        Math::Vector3::Lerp(prevScale, targetScale, scaleInterpolation));
  }
}

void NetworkTransform::SnapTransform() {
  Transform* t = entity->GetTransform();
  prevPos = targetPos;
  prevRot = targetRot;
  prevScale = targetScale;
  t->SetWorldPos(targetPos);
  t->SetWorldRot(targetRot);
  t->SetLocalScale(targetScale);
}

}  // namespace Isetta
