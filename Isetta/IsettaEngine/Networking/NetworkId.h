/*
 * Copyright (c) 2018 Isetta
 */

#pragma once
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(NetworkId, Component, true)
public:
NetworkId();
NetworkId(U32 id);

bool HasClientAuthority() const;

U32 id = 0;
int clientAuthorityId = 0;
int updateInterval = 5;
float interpolationFactor = 1;

friend class NetworkManager;
CREATE_COMPONENT_END(NetworkId, Component)
}  // namespace Isetta