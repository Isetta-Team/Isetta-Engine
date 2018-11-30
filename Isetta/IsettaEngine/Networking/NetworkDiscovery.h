/*
 * Copyright (c) 2018 Isetta
 */

#pragma once
#include <windows.networking.sockets.h>
#include "Core/DataStructures/Delegate.h"
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(NetworkDiscovery, Component, true)
public:
void FixedUpdate() override;
void OnDestroy() override;

void StartBroadcasting(const std::string &data, float duration, float interval);
void StopBroadcasting();
bool IsBroadcasting() const;

void StartListening();
void StopListening();
bool IsListening() const;

U64 AddOnMessageReceivedListener(
    const std::function<void(const char *data, const char *fromIP)>
        &onMessageReceived);
void RemoveOnMessageReceivedListener(U64& handle);
void RemoveAllOnMessageReceivedListeners();

private:
std::string broadcastContent{};
float broadcastDuration{0};
float broadcastInterval{0};
float broadcastElapsed{0};
float broadcastElapsedTotal{0};
Delegate<const char *, const char *> onMsgReceived{};

// Internal stuff
inline static u_long BLOCKING = 0;
inline static u_long NON_BLOCKING = 1;
const int PORT = 45936;
const Size BUFFER_SIZE = 2048;
char buf[2048]{};

int sizeOfAddress = sizeof(struct sockaddr_in);

// Listener stuff
void CreateListenerSocket();
void CloseListenerSocket();
void ListenToBroadcasts();
int listenerSocket{-1};

// Broadcaster stuff
void CreateBroadcasterSocket();
void CloseBroadcasterSocket();
void BroadcastMessage(std::string_view message) const;
int broadcasterSocket{-1};
DEFINE_COMPONENT_END(NetworkDiscovery, Component)
}  // namespace Isetta