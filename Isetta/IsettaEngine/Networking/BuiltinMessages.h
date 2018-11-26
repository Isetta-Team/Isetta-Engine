#pragma once
#include "Networking/Messages.h"

RPC_MESSAGE_DEFINE(ClientConnectedMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const ClientConnectedMessage* message = reinterpret_cast<const ClientConnectedMessage*>(otherMessage);
}

RPC_MESSAGE_FINISH
