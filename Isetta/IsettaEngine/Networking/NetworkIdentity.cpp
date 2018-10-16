/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkIdentity.h"
#include "Networking/NetworkManager.h"
#include "Scene/Entity.h"

namespace Isetta {
NetworkIdentity::NetworkIdentity(Entity* owner) {
  this->owner = owner;
  NetworkManager::CreateNetworkId(this);
}

NetworkIdentity::NetworkIdentity(Entity* owner, U32 id) {
  this->owner = owner;
  NetworkManager::AssignNetworkId(id, this);
}

}  // namespace Isetta