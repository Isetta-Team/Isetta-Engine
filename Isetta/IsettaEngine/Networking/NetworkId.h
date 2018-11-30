/*
 * Copyright (c) 2018 Isetta
 */

#pragma once
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(NetworkId, Component, true)
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
DEFINE_COMPONENT_END(NetworkId, Component)
}  // namespace Isetta