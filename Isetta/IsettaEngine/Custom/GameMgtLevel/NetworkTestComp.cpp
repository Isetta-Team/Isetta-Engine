/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/GameMgtLevel/NetworkTestComp.h"
#include "Core/Config/Config.h"
#include "Core/IsettaCore.h"
#include "Core/SystemInfo.h"
#include "Networking/NetworkManager.h"

namespace Isetta {
void NetworkTestComp::Start() {
  NetworkManager::Instance().AddClientConnectedListener([](const int clientId) {
    LOG_INFO(Debug::Channel::Networking, "Client %d is connected", clientId);
  });

  NetworkManager::Instance().AddClientDisconnectedListener(
      [](const int clientId) {
        LOG_INFO(Debug::Channel::Networking, "Client %d is disconnected",
                 clientId);
      });

  NetworkManager::Instance().AddConnectedToServerListener([]() {
    LOG_INFO(Debug::Channel::Networking, "Successfully connected to server");
  });

  NetworkManager::Instance().AddDisconnectedFromServerListener([]() {
    LOG_INFO(Debug::Channel::Networking,
             "Successfully disconnected from server");
  });
}

void NetworkTestComp::GuiUpdate() {
  std::string log = "Invalid";

  if (NetworkManager::Instance().IsClient()) {
    log = "Client";
  } else if (NetworkManager::Instance().IsHost()) {
    log = "Host";
  } else if (NetworkManager::Instance().IsServer()) {
    log = "Server";
  }

  static bool isOpen = true;
  GUI::Window(
      RectTransform{Math::Rect{180, 80, 200, 300}, GUI::Pivot::Top,
                    GUI::Pivot::Top},
      "Network Monitor",
      [=]() {
        GUI::Text(RectTransform{Math::Rect{5, 0, 200, 50}},
                  Util::StrFormat("Network role is %s", log.c_str()));
        GUI::Text(RectTransform{Math::Rect{5, 20, 200, 50}},
                  Util::StrFormat("MachineName: %s",
                                  SystemInfo::GetMachineName().c_str()));
        GUI::Text(RectTransform{Math::Rect{5, 40, 200, 50}},
                  Util::StrFormat("User Name: %s",
                                  SystemInfo::GetSystemUserName().c_str()));
        GUI::Text(
            RectTransform{Math::Rect{5, 60, 200, 50}},
            Util::StrFormat("IP: %s", SystemInfo::GetIpAddressWithPrefix(
                                          CONFIG_VAL(networkConfig.ipPrefix))
                                          .c_str()));
      },
      &isOpen);
}
}  // namespace Isetta