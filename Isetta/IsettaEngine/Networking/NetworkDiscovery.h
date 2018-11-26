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
inline static u_long BLOCKING = 0;
inline static u_long NON_BLOCKING = 1;
const int PORT = 45936;
const Size BUFFER_SIZE = 2048;

char buf[2048]{};

// Listener stuff
void ListenerInitialize();
void ListenerUpdate();
int sizeOfAddress = sizeof(struct sockaddr_in);
int listenerSocket{};

// Broadcaster stuff
void BroadcasterInitialize();
void BroadcastMessage(std::string_view message);
int broadcasterSocket{};
END_COMPONENT(NetworkDiscovery, Component)
}  // namespace Isetta