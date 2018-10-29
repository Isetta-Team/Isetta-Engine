/*
 * Copyright (c) 2018 Isetta
 */

#pragma once
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(NetworkIdentity, Component, true)
public:
NetworkIdentity();
NetworkIdentity(U32 id);

U32 id = 0;
U8 clientAuthority = -1;
int updateInterval = 5;

friend class NetworkManager;
END_COMPONENT(NetworkIdentity, Component)
}  // namespace Isetta