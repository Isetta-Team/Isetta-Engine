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
std::unordered_map<int, float> NetworkTransform::transformUpdateTimes;
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
            Transform& t = entity->GetTransform();
            nt->targetPos =
                t.GetParent()->GetWorldPos() + positionMessage->localPos;

            nt->posInterpolation = 0;

            if ((t.GetWorldPos() - nt->targetPos).SqrMagnitude() >=
                nt->snapDistance * nt->snapDistance) {
              t.SetLocalPos(positionMessage->localPos);
              nt->posInterpolation = 1;
            }

            nt->prevPos = t.GetWorldPos();
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
            Transform& t = entity->GetTransform();
            nt->targetRot =
                t.GetParent()->GetWorldRot() * rotationMessage->localRot;

            nt->rotInterpolation = 0;

            if (abs(Math::Quaternion::AngleDeg(
                    t.GetWorldRot(), nt->targetRot)) >= nt->snapRotation) {
              t.SetLocalRot(rotationMessage->localRot);
              nt->rotInterpolation = 1;
            }

            nt->prevRot = t.GetWorldRot();
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
            Transform& t = entity->GetTransform();
            nt->targetScale = scaleMessage->localScale;

            nt->scaleInterpolation = 0;

            if ((t.GetLocalScale() - nt->targetScale).SqrMagnitude() >=
                nt->snapScale * nt->snapScale) {
              t.SetLocalScale(scaleMessage->localScale);
              nt->scaleInterpolation = 1;
            }

            nt->prevScale = t.GetLocalScale();
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

    // Transform callbacks
    NetworkManager::Instance().RegisterClientCallback<TransformMessage>(
        [&](yojimbo::Message* message) {
          TransformMessage* transformMessage =
              reinterpret_cast<TransformMessage*>(message);

          NetworkId* netId =
              NetworkManager::Instance().GetNetworkId(transformMessage->netId);
          if (!netId || netId->HasClientAuthority()) {
            return;
          }

          Entity* entity = netId->GetEntity();
          NetworkTransform* nt = entity->GetComponent<NetworkTransform>();

          if (nt && transformMessage->updateTime < nt->lastTransformMessage) {
            return;
          }

          if (entity) {
            Transform& t = entity->GetTransform();
            nt->targetPos =
                t.GetParent()->GetWorldPos() + transformMessage->localPos;
            nt->targetRot =
                t.GetParent()->GetWorldRot() * transformMessage->localRot;
            nt->targetScale = transformMessage->localScale;

            nt->transformInterpolation = 0;

            // Currently converting the local pos to world pos, might want a
            // different way to do this
            if ((t.GetWorldPos() - nt->targetPos).SqrMagnitude() >=
                nt->snapDistance * nt->snapDistance) {
              t.SetLocalPos(transformMessage->localPos);
              t.SetLocalRot(transformMessage->localRot);
              t.SetLocalScale(transformMessage->localScale);

              nt->transformInterpolation = 1;
            }

            nt->prevPos = t.GetWorldPos();
            nt->prevRot = t.GetWorldRot();
            nt->prevScale = t.GetLocalScale();

            nt->lastTransformMessage = transformMessage->updateTime;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<TransformMessage>(
        [&](int clientIdx, yojimbo::Message* message) {
          TransformMessage* transformMessage =
              reinterpret_cast<TransformMessage*>(message);

          if (transformUpdateTimes.find(transformMessage->netId) ==
              transformUpdateTimes.end()) {
            transformUpdateTimes[transformMessage->netId] =
                transformMessage->updateTime;
          } else if (transformUpdateTimes[transformMessage->netId] <=
                     transformMessage->updateTime) {
            NetworkManager::Instance()
                .SendAllMessageFromServer<TransformMessage>(transformMessage);
          }
        });

    NetworkTransform::registeredCallbacks = true;
  }
  netId = entity->GetComponent<NetworkId>();
  targetPos = entity->GetTransform().GetWorldPos();
  prevPos = targetPos;
  targetRot = entity->GetTransform().GetWorldRot();
  prevRot = targetRot;
  targetScale = entity->GetTransform().GetLocalScale();
  prevScale = targetScale;

  lastTransformMessage = 0;
}

void NetworkTransform::FixedUpdate() {
  if (netId->HasClientAuthority()) {
    ++updateCounter;

    if (updateCounter >= netId->updateInterval) {
      updateCounter = 0;

      Transform& t = entity->GetTransform();
      // Position
      if ((t.GetWorldPos() - prevPos).SqrMagnitude() >=
          updateDistance * updateDistance) {
        PositionMessage* message =
            NetworkManager::Instance()
                .GenerateMessageFromClient<PositionMessage>();
        prevPos = t.GetWorldPos();
        message->localPos = t.GetLocalPos();
        message->netId = netId->id;
        NetworkManager::Instance().SendMessageFromClient(message);
      }
      // Rotation
      if (Math::Quaternion::AngleDeg(t.GetWorldRot(), prevRot) >=
          updateRotation) {
        RotationMessage* message =
            NetworkManager::Instance()
                .GenerateMessageFromClient<RotationMessage>();
        prevRot = t.GetWorldRot();
        message->localRot = t.GetLocalRot();
        message->netId = netId->id;
        NetworkManager::Instance().SendMessageFromClient(message);
      }
      // Scale
      if ((t.GetLocalScale() - prevScale).SqrMagnitude() >=
          updateScale * updateScale) {
        ScaleMessage* message = NetworkManager::Instance()
                                    .GenerateMessageFromClient<ScaleMessage>();
        prevScale = t.GetLocalScale();
        message->localScale = t.GetLocalScale();
        message->netId = netId->id;
        NetworkManager::Instance().SendMessageFromClient(message);
      }

      return;  // Cutting out total transform message for now
      if ((t.GetWorldPos() - targetPos).SqrMagnitude() >=
          updateDistance * updateDistance) {
        TransformMessage* message =
            NetworkManager::Instance()
                .GenerateMessageFromClient<TransformMessage>();
        targetPos = t.GetWorldPos();
        message->localPos = t.GetLocalPos();
        message->localScale = t.GetLocalScale();
        message->localRot = t.GetLocalRot();
        message->netId = netId->id;
        NetworkManager::Instance().SendMessageFromClient(message);
      }
    }
  } else {
    Transform& t = entity->GetTransform();
    float netIdLerp = 1.0 / netId->updateInterval;

    // Translation
    posInterpolation = min(posInterpolation + netIdLerp, 1);
    t.SetWorldPos(Math::Vector3::Lerp(prevPos, targetPos, posInterpolation));
    // Rotation
    rotInterpolation = min(rotInterpolation + netIdLerp, 1);
    t.SetWorldRot(Math::Quaternion::Lerp(prevRot, targetRot, rotInterpolation));
    // Scale
    scaleInterpolation = min(scaleInterpolation + netIdLerp, 1);
    t.SetLocalScale(
        Math::Vector3::Lerp(prevScale, targetScale, scaleInterpolation));

    return;  // Cutting out total transform message for now
    if (transformInterpolation < 1) {
      transformInterpolation =
          min(transformInterpolation + 1.0 / netId->updateInterval, 1);

      t.SetWorldPos(
          Math::Vector3::Lerp(prevPos, targetPos, transformInterpolation));
      t.SetWorldRot(
          Math::Quaternion::Lerp(prevRot, targetRot, transformInterpolation));
      t.SetLocalScale(
          Math::Vector3::Lerp(prevScale, targetScale, transformInterpolation));
    }
  }
}

void NetworkTransform::SnapTransform() {
  Transform& t = entity->GetTransform();
  prevPos = targetPos;
  prevRot = targetRot;
  prevScale = targetScale;
  t.SetWorldPos(targetPos);
  t.SetWorldRot(targetRot);
  t.SetLocalScale(targetScale);
}

}  // namespace Isetta
