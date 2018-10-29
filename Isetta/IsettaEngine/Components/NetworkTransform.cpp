/*
 * Copyright (c) 2018 Isetta
 */

#include "Components/NetworkTransform.h"
#include "Networking/NetworkId.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace Isetta {

bool NetworkTransform::registeredCallback = false;

void NetworkTransform::Start() {
  if (!registeredCallback) {
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

          if (entity) {
            Transform& t = entity->GetTransform();
            targetPos =
                t.GetParent()->GetWorldPos() + transformMessage->localPos;
            targetRot =
                t.GetParent()->GetWorldRot() * transformMessage->localRot;
            targetScale = transformMessage->localScale;

            interpolation = 0;

            // Currently converting the local pos to world pos, might want a
            // different way to do this
            if ((t.GetWorldPos() - targetPos).SqrMagnitude() >=
                snapDistance * snapDistance) {
              t.SetLocalPos(transformMessage->localPos);
              t.SetLocalRot(transformMessage->localRot);
              t.SetLocalScale(transformMessage->localScale);

              interpolation = 1;
            }

            prevPos = t.GetWorldPos();
            prevRot = t.GetWorldRot();
            prevScale = t.GetLocalScale();
          }
        });

    NetworkManager::Instance().RegisterServerCallback<TransformMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          TransformMessage* transformMessage =
              reinterpret_cast<TransformMessage*>(message);

          NetworkManager::Instance().SendAllMessageFromServer<TransformMessage>(
              transformMessage);
        });

    NetworkTransform::registeredCallback = true;
  }
  netId = entity->GetComponent<NetworkId>();
  targetPos = entity->GetTransform().GetWorldPos();
  prevPos = targetPos;
  targetRot = entity->GetTransform().GetWorldRot();
  prevRot = targetRot;
  targetScale = entity->GetTransform().GetLocalScale();
  prevScale = targetScale;
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
