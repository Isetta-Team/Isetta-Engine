/*
 * Copyright (c) 2018 Isetta
 */

#pragma once
#include "Scene/Component.h"
#include "Core/IsettaAlias.h"

namespace Isetta {
class NetworkIdentity : public Component {
 public:
  NetworkIdentity() = default;
  NetworkIdentity(Entity* owner);
  NetworkIdentity(Entity* owner, U32 id);

  U32 id = 0;
  U8 clientAuthority = -1;
  int updateInterval = 5;

  friend class NetworkManager;
};
 }  // namespace Isetta