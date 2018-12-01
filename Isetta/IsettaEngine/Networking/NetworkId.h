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

/// Network ID that corresponds to our entity
U32 id = 0;
/// Client ID of the client who has authority over this entity
int clientAuthorityId = 0;
/// Update interval of any networked components on this entity
int updateInterval = 5;
// Interpolation speed of any networked components on this entity
float interpolationFactor = 1;

friend class NetworkManager;
DEFINE_COMPONENT_END(NetworkId, Component)
}  // namespace Isetta