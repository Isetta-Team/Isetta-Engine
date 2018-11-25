/*
 * Copyright (c) 2018 Isetta
 */
#include "GameMgtTestComp.h"
#include "Core/IsettaCore.h"
#include "Networking/NetworkManager.h"

namespace Isetta {
void GameMgtTestComp::Start() {
  NetworkManager::Instance().AddClientConnectedListener([](const int clientId) {
    LOG_INFO(Debug::Channel::Networking, "Client %d is connected", clientId);
  });

  NetworkManager::Instance().AddClientDisconnectedListener([](const int clientId) {
    LOG_INFO(Debug::Channel::Networking, "Client %d is disconnected", clientId);
  });

  NetworkManager::Instance().AddConnectedToServerListener([]() {
    LOG_INFO(Debug::Channel::Networking, "Successfully connected to server");
  });

  NetworkManager::Instance().AddDisconnectedFromServerListener([]() {
    LOG_INFO(Debug::Channel::Networking, "Successfully disconnected from server");
  });
}

void GameMgtTestComp::FixedUpdate() {
  static U32 count = 0;
  count++;

  if (count > 180) {
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