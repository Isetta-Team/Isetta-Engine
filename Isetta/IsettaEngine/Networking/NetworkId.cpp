/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkId.h"
#include "Networking/NetworkManager.h"
#include "Scene/Entity.h"

namespace Isetta {
NetworkId::NetworkId() {
  NetworkManager::Instance().CreateNetworkId(this);
}

NetworkId::NetworkId(U32 id) {
  NetworkManager::Instance().AssignNetworkId(id, this);
}

bool NetworkId::HasClientAuthority() const {
  return clientAuthorityId ==
         NetworkManager::Instance().GetClientIndex();
}

}  // namespace Isetta