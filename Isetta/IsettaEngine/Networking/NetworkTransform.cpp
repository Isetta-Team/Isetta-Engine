/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkTransform.h"

#include <algorithm>
#include <unordered_map>
#include "Core/Time/Time.h"
#include "Networking/NetworkId.h"
#include "Scene/Entity.h"
#include "Scene/Level.h";
#include "Scene/LevelManager.h"
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
            nt->targetPos = positionMessage->localPos;

            nt->posInterpolation = 0;

            if ((Math::Vector3::Scale(t->GetWorldScale(),
                                      t->GetLocalPos() - nt->targetPos))
                    .SqrMagnitude() >= nt->snapDistance * nt->snapDistance) {
              t->SetLocalPos(positionMessage->localPos);
              nt->posInterpolation = 1;
            }

            nt->prevPos = t->GetLocalPos();
            nt->lastPosMessage = positionMessage->timestamp;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<PositionMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          PositionMessage* positionMessage =
              reinterpret_cast<PositionMessage*>(message);

          if (serverScaleTimestamps[positionMessage->netId] <=
              positionMessage->timestamp) {
            serverScaleTimestamps[positionMessage->netId] =
                positionMessage->timestamp;
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

          if (nt && rotationMessage->timestamp < nt->lastRotMessage) {
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
            nt->lastRotMessage = rotationMessage->timestamp;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<RotationMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          RotationMessage* rotationMessage =
              reinterpret_cast<RotationMessage*>(message);

          if (serverScaleTimestamps[rotationMessage->netId] <=
              rotationMessage->timestamp) {
            serverScaleTimestamps[rotationMessage->netId] =
                rotationMessage->timestamp;
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

          if (!nt) {
            return;
          }

          // Snapping
          if (transformMessage->snap) {
            Transform* t = entity->GetTransform();

            // Position
            if (transformMessage->timestamp >= nt->lastPosMessage) {
              t->SetLocalPos(transformMessage->localPos);
              nt->prevPos = t->GetLocalPos();
              nt->lastPosMessage = transformMessage->timestamp;
            }

            // Rotation
            if (transformMessage->timestamp >= nt->lastRotMessage) {
              t->SetLocalRot(transformMessage->localRot);
              nt->prevRot = t->GetLocalRot();
              nt->lastRotMessage = transformMessage->timestamp;
            }

            // Scale
            if (transformMessage->timestamp >= nt->lastScaleMessage) {
              t->SetLocalScale(transformMessage->localScale);
              nt->prevScale = t->GetLocalScale();
              nt->lastScaleMessage = transformMessage->timestamp;
            }

            nt->posInterpolation = 1;
            nt->rotInterpolation = 1;
            nt->scaleInterpolation = 1;
          } else {  // Not snapping
            Transform* t = entity->GetTransform();

            // Position
            if (transformMessage->timestamp >= nt->lastPosMessage) {
              nt->targetPos = transformMessage->localPos;

              nt->posInterpolation = 0;

              if ((Math::Vector3::Scale(t->GetWorldScale(),
                                        t->GetLocalPos() - nt->targetPos))
                      .SqrMagnitude() >= nt->snapDistance * nt->snapDistance) {
                t->SetLocalPos(transformMessage->localPos);
                nt->posInterpolation = 1;
              }

              nt->prevPos = t->GetLocalPos();
              nt->lastPosMessage = transformMessage->timestamp;
            }

            // Rotation
            if (transformMessage->timestamp >= nt->lastRotMessage) {
              nt->targetRot = transformMessage->localRot;

              nt->rotInterpolation = 0;

              if (abs(Math::Quaternion::AngleDeg(
                      t->GetLocalRot(), nt->targetRot)) >= nt->snapRotation) {
                t->SetLocalRot(transformMessage->localRot);
                nt->rotInterpolation = 1;
              }

              nt->prevRot = t->GetLocalRot();
              nt->lastRotMessage = transformMessage->timestamp;
            }

            // Scale
            if (transformMessage->timestamp >= nt->lastScaleMessage) {
              nt->targetScale = transformMessage->localScale;

              nt->scaleInterpolation = 0;

              if ((t->GetLocalScale() - nt->targetScale).SqrMagnitude() >=
                  nt->snapScale * nt->snapScale) {
                t->SetLocalScale(transformMessage->localScale);
                nt->scaleInterpolation = 1;
              }

              nt->prevScale = t->GetLocalScale();
              nt->lastScaleMessage = transformMessage->timestamp;
            }
          }
        });

    NetworkManager::Instance().RegisterServerCallback<TransformMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          TransformMessage* transformMessage =
              reinterpret_cast<TransformMessage*>(message);

          NetworkManager::Instance().SendAllMessageFromServer<TransformMessage>(
              transformMessage);
        });

    // Parenting callbacks
    NetworkManager::Instance().RegisterClientCallback<ParentMessage>(
        [](yojimbo::Message* message) {
          ParentMessage* parentMessage =
              reinterpret_cast<ParentMessage*>(message);

          NetworkId* netId =
              NetworkManager::Instance().GetNetworkId(parentMessage->netId);
          if (!netId) {
            return;
          }

          Entity* entity = netId->GetEntity();

          if (parentMessage->parentNetId == 0) {
            entity->GetTransform()->SetParent(nullptr);
          } else {
            Entity* parentEntity = NetworkManager::Instance().GetNetworkEntity(
                parentMessage->parentNetId);
            entity->GetTransform()->SetParent(parentEntity->GetTransform());
          }
        });

    NetworkManager::Instance().RegisterServerCallback<ParentMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          ParentMessage* parentMessage =
              reinterpret_cast<ParentMessage*>(message);

          NetworkManager::Instance()
              .SendAllButClientMessageFromServer<ParentMessage>(
                  clientIdx, parentMessage);
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
        message->timestamp = Time::GetElapsedTime();
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
        message->timestamp = Time::GetElapsedTime();
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
        message->timestamp = Time::GetElapsedTime();
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
        Math::Quaternion::Slerp(prevRot, targetRot, rotInterpolation));
    // Scale
    scaleInterpolation = min(scaleInterpolation + netIdLerp, 1);
    t->SetLocalScale(
        Math::Vector3::Lerp(prevScale, targetScale, scaleInterpolation));
  }
}

void NetworkTransform::ForceSendTransform(bool snap) {
  Transform* t = entity->GetTransform();

  TransformMessage* message =
      NetworkManager::Instance().GenerateMessageFromClient<TransformMessage>();
  prevPos = t->GetLocalPos();
  prevRot = t->GetLocalRot();
  prevScale = t->GetLocalScale();

  message->timestamp = Time::GetElapsedTime();
  message->localPos = t->GetLocalPos();
  message->localRot = t->GetLocalRot();
  message->localScale = t->GetLocalScale();

  NetworkManager::Instance().SendMessageFromClient(message);
}

void NetworkTransform::SetNetworkedParentToRoot() {
  ParentMessage* message =
      NetworkManager::Instance().GenerateMessageFromClient<ParentMessage>();
  message->netId = netId->id;
  message->parentNetId = 0;
  NetworkManager::Instance().SendMessageFromClient(message);

  entity->GetTransform()->SetParent(nullptr);
}

bool NetworkTransform::SetNetworkedParent(int networkId) {
  Entity* parent = NetworkManager::Instance().GetNetworkEntity(networkId);
  if (!parent) {
    return false;
  }

  ParentMessage* message =
      NetworkManager::Instance().GenerateMessageFromClient<ParentMessage>();
  message->netId = netId->id;
  message->parentNetId = networkId;
  NetworkManager::Instance().SendMessageFromClient(message);

  entity->GetTransform()->SetParent(parent->GetTransform());
  return true;
}
}  // namespace Isetta
