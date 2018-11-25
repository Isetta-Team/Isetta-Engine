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
  ServerInitialize();
  ClientInitialize();
  Input::RegisterKeyPressCallback(KeyCode::NUM1, [this]() {
    this->SendMessageToServer("Hello from the other side");
  });
}

void NetworkDiscovery::Update() { ServerUpdate(); }

void NetworkDiscovery::OnDestroy() {
  closesocket(serverSocket);
  closesocket(clientSocket);
}

void NetworkDiscovery::ServerInitialize() {
  if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    LOG_ERROR(Debug::Channel::Networking, "cannot create socket\n");
    return;
  }

  memset(reinterpret_cast<char *>(&myAddress), 0, sizeof(myAddress));
  myAddress.sin_family = AF_INET;
  myAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  myAddress.sin_port = htons(PORT);

  if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&myAddress),
           sizeof(myAddress)) < 0) {
    LOG_ERROR(Debug::Channel::Networking, "bind failed");
    return;
  }

  // If iMode = 0, blocking is enabled;
  // If iMode != 0, non-blocking mode is enabled.
  u_long mode = 1;
  int result = ioctlsocket(serverSocket, FIONBIO, &mode);
  if (result != NO_ERROR) {
    LOG_ERROR(Debug::Channel::Networking,
              "Set socket to non-blocking failed: %ld\n", result);
  }

  LOG_INFO(Debug::Channel::Networking, "Server initialized");
}

void NetworkDiscovery::ServerUpdate() {
  int msgLength = recvfrom(serverSocket, buf, BUFFER_SIZE, 0,
                           reinterpret_cast<struct sockaddr *>(&remoteAddress),
                           &addressLength);

  if (msgLength > 0) {
    buf[msgLength] = 0;
    LOG_INFO(Debug::Channel::Networking, "Received message from %s {%s}",
             inet_ntoa(remoteAddress.sin_addr), buf);
  }
}

void NetworkDiscovery::ClientInitialize() {
  clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

  struct hostent *hp = gethostbyname(SystemInfo::GetMachineName().c_str());
  memset(reinterpret_cast<char *>(&clientAddress), 0, sizeof(clientAddress));
  clientAddress.sin_family = AF_INET;
  clientAddress.sin_port = htons(PORT);

  memcpy(static_cast<void *>(&clientAddress.sin_addr), hp->h_addr_list[1],
         hp->h_length);

  u_long mode = 1;
  int result = ioctlsocket(serverSocket, FIONBIO, &mode);
  if (result != NO_ERROR) {
    LOG_ERROR(Debug::Channel::Networking,
              "Set socket to non-blocking failed: %ld\n", result);
  }
  LOG_INFO(Debug::Channel::Networking, "Client initialized");
}

void NetworkDiscovery::SendMessageToServer(std::string_view message) {
  int sendResult = sendto(
      clientSocket, message.data(), strlen(message.data()), 0,
      reinterpret_cast<struct sockaddr *>(&clientAddress), addressLength);
  if (sendResult < 0) {
    LOG_ERROR(Debug::Channel::Networking, "Send failed");
  } else {
    LOG_INFO(Debug::Channel::Networking, "Message {%s} sent", message.data());
  }
}
}  // namespace Isetta