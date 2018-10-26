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
          TransformMessage* transformMessage =
              reinterpret_cast<TransformMessage*>(message);

          NetworkId* netId =
              NetworkManager::Instance().GetNetworkId(transformMessage->netId);
          if (!netId || netId->clientAuthority == NetworkManager::Instance().GetClientIndex()) {
            return;
          }

          Entity* entity = netId->GetEntity();

          if (entity) {
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

          NetworkManager::Instance().SendAllMessageFromServer<TransformMessage>(
              transformMessage);
        });

    NetworkTransform::registeredCallback = true;
  }
  netId = entity->GetComponent<NetworkId>();
}

void NetworkTransform::FixedUpdate() {
  if (netId->clientAuthority == NetworkManager::Instance().GetClientIndex()) {
    ++updateCounter;
    if (updateCounter >= netId->updateInterval) {
      LOG("HI!");
      updateCounter = 0;
      TransformMessage* message =
          NetworkManager::Instance()
              .GenerateMessageFromClient<TransformMessage>();
      Transform t = entity->GetTransform();
      message->localPos = t.GetLocalPos();
      message->localScale = t.GetLocalScale();
      message->localRot = t.GetLocalRot();
      message->netId = netId->id;
      NetworkManager::Instance().SendMessageFromClient(message);
    }
  }
}

}  // namespace Isetta
