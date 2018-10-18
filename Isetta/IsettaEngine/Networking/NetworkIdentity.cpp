/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkIdentity.h"
#include "Networking/NetworkManager.h"
#include "Scene/Entity.h"

namespace Isetta {
NetworkIdentity::NetworkIdentity() {
  NetworkManager::Instance().CreateNetworkId(this);
}

NetworkIdentity::NetworkIdentity(U32 id) {
  NetworkManager::Instance().AssignNetworkId(id, this);
}

}  // namespace Isetta