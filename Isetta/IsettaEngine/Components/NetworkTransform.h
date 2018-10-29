/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Math.h"
#include "ISETTA_API.h"
#include "Networking/Messages.h"
#include "Scene/Component.h"
#include "Core/Config/Config.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(NetworkTransform, Component, true)
public:
void Start() override;
void FixedUpdate() override;

float updateDistance = .01;
float snapDistance = 100;

private:
int updateCounter = 0;
float interpolation = 1;
Math::Vector3 targetPos;
Math::Vector3 prevPos;
Math::Quaternion targetRot;
Math::Quaternion prevRot;
Math::Vector3 targetScale;
Math::Vector3 prevScale;
static bool registeredCallback;
class NetworkId* netId;
friend class NetworkTransform;
CREATE_COMPONENT_END(NetworkTransform, Component)

RPC_MESSAGE_DEFINE(TransformMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, Config::Instance().networkConfig.maxNetID.GetVal());

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
}

public:
int netId;
Math::Vector3 localPos;
Math::Vector3 localScale;
Math::Quaternion localRot;

RPC_MESSAGE_FINISH

}  // namespace Isetta
