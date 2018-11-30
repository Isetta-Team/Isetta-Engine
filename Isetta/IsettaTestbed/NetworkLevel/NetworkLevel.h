/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>

/**
 * @brief Level demoing some of our networking capabilities. The
 * `default_server_ip` in config should be set to your LAN IP for this level to
 * work.
 *
 */
namespace Isetta {
DEFINE_LEVEL(NetworkLevel)
void Load() override;
};  // namespace Isetta

// HandleMessage simply sends an integer handle between 0 and 64 across the
// network
DEFINE_NETWORK_MESSAGE(HandleMessage)

// IMPORTANT: The Serialize function _must_ be a template around the Stream
// type, and it MUST return true at the end of the function. Otherwise the
// serialization will be assumed to have failed!
template <typename Stream>
bool Serialize(Stream* stream) {
  // serialize_int is supplied by yojimbo, along with many other primitive
  // type serialization functions
  serialize_int(stream, handle, 0, 64);
  return true;
}

// The Copy function must be overridden with boilerplate that copies the values
// from a given message. This is used for the general SendToAll functions that
// the NetworkManager has.
void Copy(const yojimbo::Message* otherMessage) override {
  const HandleMessage* message =
      reinterpret_cast<const HandleMessage*>(otherMessage);
  handle = message->handle;
}

public:
int handle = 0;  // Obviously we'll just use a handle

DEFINE_NETWORK_MESSAGE_END

// SpawnMessage not only tells the machine to spawn a character, but also
// grants it a network ID, clientAuthority, and possibly a parent network ID
DEFINE_NETWORK_MESSAGE(SpawnMessage)

// Again, we HAVE to implement this function...
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);
  serialize_int(stream, clientAuthorityId, 0,
                NetworkManager::Instance().GetMaxClients());
  serialize_int(stream, parentId, 0, 256);
  serialize_float(stream, b);  // These are not really used, they're just filler
                               // for the sake of filler
  serialize_float(stream, c);

  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const SpawnMessage* message =
      reinterpret_cast<const SpawnMessage*>(otherMessage);
  netId = message->netId;
  clientAuthorityId = message->clientAuthorityId;
  parentId = message->parentId;
  b = message->b;
  c = message->c;
}

// Network ID tells the clients what ID the server has granted an entity
int netId = 0;
// Client authority ID tells the clients which of them can influence an
// object (which is nice for NetworkTransform)
int clientAuthorityId = 0;
// Parent ID tells the clients which entity should be the parent of the
// spawned entity
int parentId = 0;
float b = 0, c = 0;  // These are unimportant, they're just testing values

DEFINE_NETWORK_MESSAGE_END

// DespawnMessage pretty much just tells the machine who to despawn
DEFINE_NETWORK_MESSAGE(DespawnMessage)

DespawnMessage() { netId = 0; }

template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);

  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const DespawnMessage* message =
      reinterpret_cast<const DespawnMessage*>(otherMessage);
  netId = message->netId;
}

int netId;

DEFINE_NETWORK_MESSAGE_END
}  // namespace Isetta