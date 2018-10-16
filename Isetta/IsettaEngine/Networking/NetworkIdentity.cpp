/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkIdentity.h"

namespace Isetta {
NetworkIdentity::NetworkIdentity() { NetworkRegistry::CreateNetworkId(this); }

NetworkIdentity::NetworkIdentity(U32 id) {
  NetworkRegistry::AssignNetworkId(id, this);
}

}  // namespace Isetta