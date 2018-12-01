#pragma once
#include "Core/Config/Config.h"
#include "Networking/Messages.h"

/**
 * @brief ClientConnectedMessage is a network message that is sent from the
 * client to the server when the client successfully connects to the server.
 *
 */
DEFINE_NETWORK_MESSAGE(ClientConnectedMessage)
private:
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_string(stream, ip, sizeof(ip));
  serialize_string(stream, machineName, sizeof(machineName));
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const ClientConnectedMessage* message =
      reinterpret_cast<const ClientConnectedMessage*>(otherMessage);

  strcpy_s(ip, message->ip);
  strcpy_s(machineName, message->machineName);
}

public:
char ip[16];           // The IP address of the client
char machineName[16];  // The machine name of the client

DEFINE_NETWORK_MESSAGE_END

/**
 * @brief LoadLevelMessage is a network message that is sent from the server to
 * the client to signal that a level should be loaded on each of the clients.
 *
 */
DEFINE_NETWORK_MESSAGE(LoadLevelMessage)
private:
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_string(stream, levelName, sizeof(levelName));
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  auto* message = reinterpret_cast<const LoadLevelMessage*>(otherMessage);
  strcpy_s(levelName, message->levelName);
}

public:
inline const static int levelNameMaxLength = 64;
char levelName[levelNameMaxLength];  // Name of the level to be loaded

DEFINE_NETWORK_MESSAGE_END
