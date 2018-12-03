/*
 * Copyright (c) 2018 Isetta
 */
#include "NetworkTestComp.h"

#include "Core/SystemInfo.h"

namespace Isetta {
void NetworkTestComp::Start() {
  // Register a callback called when a client is connected to this server
  // Only called when running server
  NetworkManager::Instance().RegisterClientConnectedCallback([](ClientInfo info) {
    LOG_INFO(Debug::Channel::Networking,
             "Client [%s] with IP [%s] is connected", info.machineName.c_str(),
             info.ip.c_str());
  });

  // Register a callback called when a client is disconnected from this server
  // Only called when running server
  NetworkManager::Instance().RegisterClientDisconnectedCallback([](ClientInfo info) {
    LOG_INFO(Debug::Channel::Networking,
             "Client [%s] with IP [%s] is disconnected",
             info.machineName.c_str(), info.ip.c_str());
  });

  // Register a callback that's called when this client is connected to a server
  // Only called when running client
  NetworkManager::Instance().RegisterConnectedToServerCallback([]() {
    LOG_INFO(Debug::Channel::Networking, "Successfully connected to server");
  });

  // Register a callback that's called when this clinet is dropped out of server
  // Only called when running client
  NetworkManager::Instance().RegisterDisconnectedFromServerCallback([]() {
    LOG_INFO(Debug::Channel::Networking,
             "Successfully disconnected from server");
  });

  // Start as host using IP in config
  Input::RegisterKeyPressCallback(KeyCode::F1, []() {
    NetworkManager::Instance().StartHost(
        CONFIG_VAL(networkConfig.defaultServerIP));
  });

  // Start as server using IP in config
  Input::RegisterKeyPressCallback(KeyCode::F2, []() {
    NetworkManager::Instance().StartServer(
        CONFIG_VAL(networkConfig.defaultServerIP));
  });

  // Start/stop client by connecting/disconnecting server with IP in config
  Input::RegisterKeyPressCallback(KeyCode::F3, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      NetworkManager::Instance().StopClient();
    } else {
      NetworkManager::Instance().StartClient(
          CONFIG_VAL(networkConfig.defaultServerIP));
    }
  });

  // Start broadcasting a message
  Input::RegisterKeyPressCallback(KeyCode::NUM1, [this]() {
    this->entity->GetComponent<NetworkDiscovery>()->StartBroadcasting(
        "Hello from the other side", 60, 1);
    LOG_INFO(Debug::Channel::Networking,
             "Broadcasting started (duration=%.1fs, interval=%.1fs)", 60.f,
             1.f);
  });

  // Start listening to a message,
  // and add a callback when a broad message is received
  Input::RegisterKeyPressCallback(KeyCode::NUM2, [this]() {
    this->entity->GetComponent<NetworkDiscovery>()->StartListening();
    this->entity->GetComponent<NetworkDiscovery>()
        ->AddOnMessageReceivedListener([](const char* data, const char* ip) {
          LOG_INFO(Debug::Channel::Networking, "[%s] said {%s}", ip, data);
        });
    LOG_INFO(Debug::Channel::Networking, "Started listening to broadcasts");
  });

  // Let this server and all clients load level "NetworkLevel" 
  Input::RegisterKeyPressCallback(KeyCode::KP_7, []() {
    NetworkManager::Instance().NetworkLoadLevel("NetworkLevel");
  });

  // Let this server and all clients load level "EmptyLevelForNetworkLoadLevel" 
  Input::RegisterKeyPressCallback(KeyCode::KP_8, []() {
    NetworkManager::Instance().NetworkLoadLevel("EmptyLevelForNetworkLoadLevel");
  });
}

}  // namespace Isetta