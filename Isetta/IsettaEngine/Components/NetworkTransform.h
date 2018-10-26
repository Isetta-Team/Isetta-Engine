/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Math.h"
#include "ISETTA_API.h"
#include "Networking/Messages.h"
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(NetworkTransform, Component, true)
public:
void Start() override;
void FixedUpdate() override;

private:
int updateCounter = 0;
static bool registeredCallback;
class NetworkId* netId;
CREATE_COMPONENT_END(NetworkTransform, Component)

RPC_MESSAGE_DEFINE(TransformMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_uint32(stream, netId);

  serialize_float(stream, localPos.x);
  serialize_float(stream, localPos.y);
  serialize_float(stream, localPos.z);

  serialize_float(stream, localScale.x);
  serialize_float(stream, localScale.y);
  serialize_float(stream, localScale.z);

  serialize_float(stream, localRot.x);
  serialize_float(stream, localRot.y);
  serialize_float(stream, localRot.z);
  serialize_float(stream, localRot.w);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const TransformMessage* message =
      reinterpret_cast<const TransformMessage*>(otherMessage);

  netId = message->netId;
  localPos = message->localPos;
  localScale = message->localScale;
  localRot = message->localRot;
  // copy stuff here
}

public:
U32 netId;
Math::Vector3 localPos;
Math::Vector3 localScale;
Math::Quaternion localRot;

RPC_MESSAGE_FINISH

}  // namespace Isetta
