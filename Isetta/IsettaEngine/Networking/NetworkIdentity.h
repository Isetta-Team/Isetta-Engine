/*
 * Copyright (c) 2018 Isetta
 */

#pragma once
#include "Networking/Messages.h"
#include "Scene/Component.h"

namespace Isetta {
class NetworkIdentity : public Component {
 public:
  NetworkIdentity();
  NetworkIdentity(U32 id);

  U32 id;
  U8 clientAuthority = -1;
  int updateInterval = 5;

  friend class NetworkRegistry;
};
 }  // namespace Isetta