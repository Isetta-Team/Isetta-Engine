/*
 * Copyright (c) 2018 Isetta
 */
#include "GameMgtTestComp.h"
#include "Core/IsettaCore.h"
#include "Networking/NetworkManager.h"

namespace Isetta {
void GameMgtTestComp::FixedUpdate() {
  static U32 count = 0;
  count++;

  if (count > 60) {
    std::string log = "Invalid";

    if (NetworkManager::Instance().IsClient()) {
      log = "Client";
    } else if (NetworkManager::Instance().IsHost()) {
      log = "Host";
    } else if (NetworkManager::Instance().IsServer()) {
      log = "Server";
    }
    
    LOG_INFO(Debug::Channel::Networking, "Network role is %s", log.c_str());
    count = 0;
  }
}
}  // namespace Isetta