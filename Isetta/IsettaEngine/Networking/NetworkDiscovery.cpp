/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkDiscovery.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/SystemInfo.h"
#include "Input/Input.h"

namespace Isetta {

void NetworkDiscovery::Start() {
  ListenerInitialize();
  BroadcasterInitialize();

  Input::RegisterKeyPressCallback(KeyCode::NUM1, [this]() {
    this->BroadcastMessage("Hello from the other side");
  });
}

void NetworkDiscovery::Update() { ListenerUpdate(); }

void NetworkDiscovery::OnDestroy() {
  closesocket(listenerSocket);
  closesocket(broadcasterSocket);
}

void NetworkDiscovery::ListenerInitialize() {
  listenerSocket = socket(AF_INET, SOCK_DGRAM, 0);
  ASSERT(listenerSocket >= 0);

  int result = ioctlsocket(listenerSocket, FIONBIO, &NON_BLOCKING);
  ASSERT(result == NO_ERROR);

  char broadcast = '1';
  if (setsockopt(listenerSocket, SOL_SOCKET, SO_BROADCAST, &broadcast,
                 sizeof(broadcast)) < 0) {
    LOG_ERROR(Debug::Channel::Networking, "Error in setting Broadcast option");
  }

  struct sockaddr_in listenerAddress {};
  memset(reinterpret_cast<char *>(&listenerAddress), 0,
         sizeof(listenerAddress));
  listenerAddress.sin_family = AF_INET;
  listenerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  listenerAddress.sin_port = htons(PORT);

  int bindResult = bind(listenerSocket,
                        reinterpret_cast<struct sockaddr *>(&listenerAddress),
                        sizeOfAddress);
  ASSERT(bindResult >= 0);

  LOG_INFO(Debug::Channel::Networking, "Listener initialized");
}

void NetworkDiscovery::ListenerUpdate() {
  struct sockaddr_in fromAddress;
  int msgLength = recvfrom(listenerSocket, buf, BUFFER_SIZE, 0,
                           reinterpret_cast<struct sockaddr *>(&fromAddress),
                           &sizeOfAddress);
  if (msgLength > 0) {
    buf[msgLength] = 0;
    LOG_INFO(Debug::Channel::Networking, "Received message from %s {%s}",
             inet_ntoa(fromAddress.sin_addr), buf);
  }
}

void NetworkDiscovery::BroadcasterInitialize() {
  broadcasterSocket = socket(AF_INET, SOCK_DGRAM, 0);
  int result = ioctlsocket(broadcasterSocket, FIONBIO, &NON_BLOCKING);
  ASSERT(result == NO_ERROR);

  char broadcastPermission = '1';
  setsockopt(broadcasterSocket, SOL_SOCKET, SO_BROADCAST, &broadcastPermission,
             sizeof(broadcastPermission));

  struct sockaddr_in broadcasterAddress {};
  memset(reinterpret_cast<char *>(&broadcasterAddress), 0,
         sizeof(broadcasterAddress));
  broadcasterAddress.sin_family = AF_INET;
  broadcasterAddress.sin_port = htons(PORT);
  broadcasterAddress.sin_addr.s_addr = inet_addr(
      SystemInfo::GetIpAddressWithPrefix(CONFIG_VAL(networkConfig.ipPrefix))
          .c_str());

  int bindResult = bind(
      broadcasterSocket,
      reinterpret_cast<struct sockaddr *>(&broadcasterAddress), sizeOfAddress);
  ASSERT(bindResult >= 0);

  LOG_INFO(Debug::Channel::Networking, "Broadcaster initialized");
}

void NetworkDiscovery::BroadcastMessage(std::string_view message) {
  struct sockaddr_in targetAddress;
  memset(reinterpret_cast<char *>(&targetAddress), 0, sizeOfAddress);
  targetAddress.sin_family = AF_INET;
  targetAddress.sin_port = htons(PORT);
  targetAddress.sin_addr.s_addr = inet_addr("255.255.255.255");

  int sendResult = sendto(
      broadcasterSocket, message.data(), strlen(message.data()), 0,
      reinterpret_cast<struct sockaddr *>(&targetAddress), sizeOfAddress);

  if (sendResult < 0) {
    LOG_ERROR(Debug::Channel::Networking, "Send failed");
  } else {
    LOG_INFO(Debug::Channel::Networking, "Message {%s} sent", message.data());
  }
}
}  // namespace Isetta