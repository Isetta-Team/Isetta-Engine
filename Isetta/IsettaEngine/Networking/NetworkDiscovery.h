/*
 * Copyright (c) 2018 Isetta
 */

#pragma once
#include <windows.networking.sockets.h>
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(NetworkDiscovery, Component, true)
public:
void Start() override;
void Update() override;
void OnDestroy() override;

private:
const Size BUFFER_SIZE = 2048;
const int PORT = 45936;

// Server stuff
void ServerInitialize();
void ServerUpdate();
struct sockaddr_in myAddress {}; /* our address */
struct sockaddr_in remoteAddress {};
int addressLength = sizeof(myAddress); /* length of addresses */
int serverSocket{};                    /* our socket */
char buf[2048]{};                      /* receive buffer */

// Client stuff
void ClientInitialize();
void SendMessageToServer(std::string_view message);
int clientSocket{};
struct sockaddr_in clientAddress {};
END_COMPONENT(NetworkDiscovery, Component)
}  // namespace Isetta