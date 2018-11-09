/*
 * Copyright (c) 2018 Isetta
 */

#pragma once
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(NetworkId, Component, true)
public:
NetworkId();
NetworkId(U32 id);

void OnDestroy() override;

bool HasClientAuthority() const;

U32 id = 0;
int clientAuthorityId = 0;
int updateInterval = 5;
float interpolationFactor = 1;

friend class NetworkManager;
END_COMPONENT(NetworkId, Component)
}  // namespace Isetta