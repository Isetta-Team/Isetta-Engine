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

/**
 * @brief Begins broadcasting a message to the local area network through a
 * predefined socket at a regular interval.
 *
 * @param data The message to broadcast
 * @param duration The length of time to broadcast for until timing out
 * @param interval The interval between message broadcasts
 */
void StartBroadcasting(const std::string &data, float duration, float interval);
void StopBroadcasting();
bool IsBroadcasting() const;

/**
 * @brief Begins listening for messages on the local area network through a
 * predefined socket.
 *
 */
void StartListening();
void StopListening();
bool IsListening() const;

/**
 * @brief Registers a callback that is run when a broadcasted message is
 * received on a listening machine.
 *
 * @param onMessageReceived The function that is run when the message is
 * received
 * @return U64 The handle that is registered to the callback if we want to
 * deregister it.
 */
U64 AddOnMessageReceivedListener(
    const std::function<void(const char *data, const char *fromIP)>
        &onMessageReceived);
/**
 * @brief Removes a registered callback from running when a broadcasted message
 * is received.
 *
 * @param handle Handle corresponding to the callback function to be removed.
 */
void RemoveOnMessageReceivedListener(U64 &handle);
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