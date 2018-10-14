/*
 * Copyright (c) 2018 Isetta
 */

#pragma once

#include "Networking/Messages.h"
#include "Core/Debug/Logger.h"
#include "Audio/AudioSource.h"
#include "Networking/NetworkManager.h"


namespace Isetta {

/**
 * @brief Code-generated struct to be used for sending integer values across the
 * network.
 *
 */
RPC_MESSAGE_DEFINE(HandleMessage)
HandleMessage() { handle = 0; }

// TODO(Caleb): choose a more reasonable range for the int serialization
template <typename Stream>
bool Serialize(Stream& stream) {
  serialize_int(stream, handle, 0, 64);

  return true;
}

public:
int handle;

RPC_CLIENT_FUNC {
  HandleMessage* handleMessage = static_cast<HandleMessage*>(message);
  LOG(Debug::Channel::Networking, "Server sends handle #%d",
      handleMessage->handle);
  if (handleMessage->handle == 0) {
    LOG(Debug::Channel::Networking,
        "Server says we should play the animation!");
  }
  if (handleMessage->handle == 1) {
    LOG(Debug::Channel::Networking,
        "Server says we should stop the animation!");
  }
  if (handleMessage->handle == 2) {
    AudioSource audio = AudioSource();
    audio.SetAudioClip("gunshot.aiff");
    audio.Play(false, 1.f);
  }
}

RPC_SERVER_FUNC {
  HandleMessage* handleMessage = reinterpret_cast<HandleMessage*>(message);
  LOG(Debug::Channel::Networking, "Client %d sends handle #%d", clientIdx,
      handleMessage->handle);
  for (int i = 0; i < server->GetMaxClients(); i++) {
    if (!server->IsClientConnected(i)) {
      continue;
    }
    HandleMessage* newMessage =
        reinterpret_cast<HandleMessage*>(NetworkManager::GenerateMessageFromServer(i, "HNDL"));
    newMessage->handle = handleMessage->handle;
    NetworkManager::SendMessageFromServer(i, newMessage);
  }
}
RPC_MESSAGE_FINISH

/**
 * @brief Code-generated struct to be used for sending string messages across
 * the network.
 *
 */
RPC_MESSAGE_DEFINE(StringMessage)

StringMessage() { string = ""; }

// TODO(Caleb): choose a more reasonable range for the int serialization
template <typename Stream>
bool Serialize(Stream& stream) {
  serialize_string(stream, const_cast<char*>(string.c_str()), 512);

  return true;
}

public:
std::string string;

RPC_CLIENT_FUNC {
  StringMessage* stringMessage = static_cast<StringMessage*>(message);
  LOG(Debug::Channel::Networking, "Server says: %s",
      stringMessage->string.c_str());
}

RPC_SERVER_FUNC {
  StringMessage* stringMessage = reinterpret_cast<StringMessage*>(message);
  LOG(Debug::Channel::Networking, "Client %d says: %s", clientIdx,
      stringMessage->string.c_str());
}

RPC_MESSAGE_FINISH

inline void InitExampleMessages() {
  RPC_MESSAGE_INIT(HandleMessage, "HNDL");
  RPC_MESSAGE_INIT(StringMessage, "STRN");
}

}