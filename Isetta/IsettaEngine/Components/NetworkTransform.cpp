/*
 * Copyright (c) 2018 Isetta
 */

#include "Components/NetworkTransform.h"
#include "Networking/NetworkId.h"
#include "Scene/Entity.h"

namespace Isetta {

bool NetworkTransform::registeredCallback = false;

void NetworkTransform::Start() {
  if (!registeredCallback) {
    NetworkManager::Instance().RegisterClientCallback<TransformMessage>(
        [](yojimbo::Message* message) {
          if (NetworkManager::Instance().ServerIsRunning()) {
            return;
          }

          TransformMessage* transformMessage =
              reinterpret_cast<TransformMessage*>(message);
          Entity* entity = NetworkManager::Instance().GetNetworkEntity(
              transformMessage->netId);

          LOG(Util::StrFormat("%f, %f, %f", transformMessage->localPos.x,
                              transformMessage->localPos.y,
                              transformMessage->localPos.z));
          LOG(Util::StrFormat("NetId: %d", transformMessage->netId));
          if (entity) {
            LOG(Util::StrFormat("Entity exists!"));
            Transform t = entity->GetTransform();
            t.SetLocalPos(transformMessage->localPos);
            t.SetLocalScale(transformMessage->localScale);
            t.SetLocalRot(transformMessage->localRot);
          }
        });

    NetworkManager::Instance().RegisterServerCallback<TransformMessage>(
        [](int clientIdx, yojimbo::Message* message) {
          TransformMessage* transformMessage =
              reinterpret_cast<TransformMessage*>(message);

          Entity* entity = NetworkManager::Instance().GetNetworkEntity(
              transformMessage->netId);
          if (entity) {
            Transform t = entity->GetTransform();
            t.SetLocalPos(transformMessage->localPos);
            t.SetLocalScale(transformMessage->localScale);
            t.SetLocalRot(transformMessage->localRot);
          }

          NetworkManager::Instance().SendAllMessageFromServer<TransformMessage>(
              transformMessage);
        });

    NetworkTransform::registeredCallback = true;
  }
  netId = entity->GetComponent<NetworkId>();
}

void NetworkTransform::FixedUpdate() {
  ++updateCounter;
  if (updateCounter >= netId->updateInterval) {
    updateCounter = 0;
    TransformMessage* message =
        NetworkManager::Instance()
            .GenerateMessageFromClient<TransformMessage>();
    Transform t = entity->GetTransform();
    message->localPos = t.GetLocalPos();
    message->localScale = t.GetLocalScale();
    message->localRot = t.GetLocalRot();
    NetworkManager::Instance().SendMessageFromClient(message);
  }
}

}  // namespace Isetta
