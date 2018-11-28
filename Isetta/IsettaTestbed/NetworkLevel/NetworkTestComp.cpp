/*
 * Copyright (c) 2018 Isetta
 */
#include "NetworkTestComp.h"

#include "Core/Config/Config.h"
#include "Core/IsettaCore.h"
#include "Core/SystemInfo.h"
#include "Networking/NetworkDiscovery.h"
#include "Networking/NetworkManager.h"

namespace Isetta {
void NetworkTestComp::Start() {
  NetworkManager::Instance().AddClientConnectedListener([](ClientInfo info) {
    LOG_INFO(Debug::Channel::Networking,
             "Client [%s] with IP [%s] is connected", info.machineName.c_str(),
             info.ip.c_str());
  });

  NetworkManager::Instance().AddClientDisconnectedListener([](ClientInfo info) {
    LOG_INFO(Debug::Channel::Networking,
             "Client [%s] with IP [%s] is disconnected",
             info.machineName.c_str(), info.ip.c_str());
  });

  NetworkManager::Instance().AddConnectedToServerListener([]() {
    LOG_INFO(Debug::Channel::Networking, "Successfully connected to server");
  });

  NetworkManager::Instance().AddDisconnectedFromServerListener([]() {
    LOG_INFO(Debug::Channel::Networking,
             "Successfully disconnected from server");
  });

  Input::RegisterKeyPressCallback(KeyCode::F1, []() {
    NetworkManager::Instance().StartHost(
        CONFIG_VAL(networkConfig.defaultServerIP));
  });

  Input::RegisterKeyPressCallback(KeyCode::F2, []() {
    NetworkManager::Instance().StartServer(
        CONFIG_VAL(networkConfig.defaultServerIP));
  });

  Input::RegisterKeyPressCallback(KeyCode::F3, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      NetworkManager::Instance().StopClient();
    } else {
      NetworkManager::Instance().StartClient(
          CONFIG_VAL(networkConfig.defaultServerIP));
    }
  });

  Input::RegisterKeyPressCallback(KeyCode::NUM1, [this]() {
    this->entity->GetComponent<NetworkDiscovery>()->StartBroadcasting(
        "Hello from the other side", 60, 1);
    LOG_INFO(Debug::Channel::Networking,
             "Broadcasting started (duration=%.1fs, interval=%.1fs)", 60.f,
             1.f);
  });

  Input::RegisterKeyPressCallback(KeyCode::NUM2, [this]() {
    this->entity->GetComponent<NetworkDiscovery>()->StartListening();
    this->entity->GetComponent<NetworkDiscovery>()
        ->AddOnMessageReceivedListener([](const char* data, const char* ip) {
          LOG_INFO(Debug::Channel::Networking, "[%s] said {%s}", ip, data);
        });
    LOG_INFO(Debug::Channel::Networking, "Started listening to broadcasts");
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