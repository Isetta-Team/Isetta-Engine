/*
 * Copyright (c) 2018 Isetta
 */

#include "Components/NetworkTransform.h"
#include "Networking/NetworkId.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace Isetta {

bool NetworkTransform::registeredCallbacks = false;
std::unordered_map<int, float> NetworkTransform::updateTimes;

void NetworkTransform::Start() {
  if (!registeredCallbacks) {
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

          if (transformMessage->updateTime < nt->lastUpdateTime) {
            return;
          }

          if (entity) {
            Transform& t = entity->GetTransform();
            nt->targetPos =
                t.GetParent()->GetWorldPos() + transformMessage->localPos;
            nt->targetRot =
                t.GetParent()->GetWorldRot() * transformMessage->localRot;
            nt->targetScale = transformMessage->localScale;

            nt->interpolation = 0;

            // Currently converting the local pos to world pos, might want a
            // different way to do this
            float l = (t.GetWorldPos() - nt->targetPos).SqrMagnitude();
            if ((t.GetWorldPos() - nt->targetPos).SqrMagnitude() >=
                nt->snapDistance * nt->snapDistance) {
              t.SetLocalPos(transformMessage->localPos);
              t.SetLocalRot(transformMessage->localRot);
              t.SetLocalScale(transformMessage->localScale);

              nt->interpolation = 1;
            }

            nt->prevPos = t.GetWorldPos();
            nt->prevRot = t.GetWorldRot();
            nt->prevScale = t.GetLocalScale();

            nt->lastUpdateTime = transformMessage->updateTime;
          }
        });

    NetworkManager::Instance().RegisterServerCallback<TransformMessage>(
        [&](int clientIdx, yojimbo::Message* message) {
          TransformMessage* transformMessage =
              reinterpret_cast<TransformMessage*>(message);

          if (updateTimes.find(transformMessage->netId) ==
              updateTimes.end()) {
            updateTimes[transformMessage->netId] = transformMessage->updateTime;
          } else if (updateTimes[transformMessage->netId] <=
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

  lastUpdateTime = 0;
}

void NetworkTransform::FixedUpdate() {
  if (netId->HasClientAuthority()) {
    ++updateCounter;

    if (updateCounter >= netId->updateInterval) {
      updateCounter = 0;

      Transform& t = entity->GetTransform();
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
  } else if (interpolation < 1) {
    Transform& t = entity->GetTransform();
    interpolation = min(interpolation + 1.0 / netId->updateInterval, 1);
    t.SetWorldPos(Math::Vector3::Lerp(prevPos, targetPos, interpolation));
    //t.SetWorldRot(Math::Quaternion::Slerp(prevRot, targetRot, interpolation));
    t.SetLocalScale(Math::Vector3::Lerp(prevScale, targetScale, interpolation));
  }
}

}  // namespace Isetta
