/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkDiscovery.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/SystemInfo.h"
#include "Core/Time/Time.h"

namespace Isetta {

void NetworkDiscovery::FixedUpdate() {
  if (IsListening()) {
    ListenToBroadcasts();
  }

  if (IsBroadcasting()) {
    if (broadcastElapsedTotal <= broadcastDuration) {
      if (broadcastElapsed >= broadcastInterval) {
        BroadcastMessage(broadcastContent);
        broadcastElapsed -= broadcastInterval;
      }
      broadcastElapsed += Time::GetFixedDeltaTime();
      broadcastElapsedTotal += Time::GetFixedDeltaTime();
    } else {
      StopBroadcasting();
    }
  }
}

void NetworkDiscovery::OnDestroy() {
  if (IsListening()) CloseListenerSocket();
  if (IsBroadcasting()) CloseBroadcasterSocket();
}

void NetworkDiscovery::StartBroadcasting(const std::string &data,
                                         const float duration,
                                         const float interval) {
  if (data.length() + 1 >= BUFFER_SIZE) {
    LOG_ERROR(Debug::Channel::Networking,
              "byteLength too big, consider broadcasting smaller messages");
    return;
  }

  if (IsBroadcasting()) {
    LOG_WARNING(
        Debug::Channel::Networking,
        "There was another broadcaster running, terminated the old one");
    CloseBroadcasterSocket();
  }

  CreateBroadcasterSocket();
  broadcastContent = data;
  broadcastDuration = duration;
  broadcastInterval = interval;
  broadcastElapsed = interval;
  broadcastElapsedTotal = 0.f;
}

void NetworkDiscovery::StopBroadcasting() {
  if (!IsBroadcasting()) {
    LOG_WARNING(Debug::Channel::Networking,
                "Broadcaster is not running, there's nothing to close");
    return;
  }

  CloseBroadcasterSocket();
  broadcastContent.clear();
  broadcastDuration = 0;
  broadcastInterval = 0;
  broadcastElapsed = 0.f;
  broadcastElapsedTotal = 0.f;
}

bool NetworkDiscovery::IsBroadcasting() const {
  return broadcasterSocket != -1;
}

void NetworkDiscovery::StartListening() {
  if (IsListening()) {
    LOG_WARNING(Debug::Channel::Networking, "Listener already running");
    return;
  }
  CreateListenerSocket();
}

U64 NetworkDiscovery::AddOnMessageReceivedListener(
    const std::function<void(const char *data, const char *fromIP)>
        &onMessageReceived) {
  return onMsgReceived.Subscribe(onMessageReceived);
}

void NetworkDiscovery::RemoveOnMessageReceivedListener(U64& handle) {
  onMsgReceived.Unsubscribe(handle);
}

void NetworkDiscovery::RemoveAllOnMessageReceivedListeners() {
  onMsgReceived.Clear();
}

void NetworkDiscovery::StopListening() {
  if (!IsListening()) {
    LOG_WARNING(Debug::Channel::Networking,
                "Listener is not running, nothing to close");
    return;
  }
  CloseListenerSocket();
}

bool NetworkDiscovery::IsListening() const { return listenerSocket != -1; }

void NetworkDiscovery::CreateListenerSocket() {
  listenerSocket = socket(AF_INET, SOCK_DGRAM, 0);

  int result = ioctlsocket(listenerSocket, FIONBIO, &NON_BLOCKING);
  if (result != NO_ERROR) {
    LOG_ERROR(Debug::Channel::Networking,
              "Failed to set socket to non-blocking");
  }

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
  if (bindResult < 0) {
    LOG_ERROR(Debug::Channel::Networking, "Error binding broadcaster socket");
  }
}

void NetworkDiscovery::ListenToBroadcasts() {
  struct sockaddr_in fromAddress {};
  int msgLength = recvfrom(listenerSocket, buf, BUFFER_SIZE, 0,
                           reinterpret_cast<struct sockaddr *>(&fromAddress),
                           &sizeOfAddress);

  if (msgLength > 0) {
    buf[msgLength] = 0;
    onMsgReceived.Invoke(buf, inet_ntoa(fromAddress.sin_addr));
  }
}

void NetworkDiscovery::CloseListenerSocket() {
  closesocket(listenerSocket);
  listenerSocket = -1;
}

void NetworkDiscovery::CreateBroadcasterSocket() {
  broadcasterSocket = socket(AF_INET, SOCK_DGRAM, 0);
  int result = ioctlsocket(broadcasterSocket, FIONBIO, &NON_BLOCKING);
  if (result != NO_ERROR) {
    LOG_ERROR(Debug::Channel::Networking,
              "Failed to set socket to non-blocking");
  }

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
  if (bindResult < 0) {
    LOG_ERROR(Debug::Channel::Networking, "Error binding broadcaster socket");
  }
}

void NetworkDiscovery::CloseBroadcasterSocket() {
  closesocket(broadcasterSocket);
  broadcasterSocket = -1;
}

void NetworkDiscovery::BroadcastMessage(std::string_view message) const {
  ASSERT(IsBroadcasting());
  struct sockaddr_in targetAddress {};
  memset(reinterpret_cast<char *>(&targetAddress), 0, sizeOfAddress);
  targetAddress.sin_family = AF_INET;
  targetAddress.sin_port = htons(PORT);
  targetAddress.sin_addr.s_addr = inet_addr("255.255.255.255");

  int sendResult = sendto(
      broadcasterSocket, message.data(), strlen(message.data()), 0,
      reinterpret_cast<struct sockaddr *>(&targetAddress), sizeOfAddress);

  if (sendResult < 0) {
    LOG_ERROR(Debug::Channel::Networking, "Send failed");
  }
}
}  // namespace Isetta