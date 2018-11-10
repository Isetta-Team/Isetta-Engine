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
BEGIN_COMPONENT(NetworkTransform, Component, true)
public:
void Start() override;
void FixedUpdate() override;

void ForceSendTransform();
void SnapTransform();
bool SetNetworkedParent(int netId);
void SetNetworkedParentToRoot();

float updateDistance = .01;
float updateRotation = 5;
float updateScale = .005;
float snapDistance = 5;
float snapRotation = 30;
float snapScale = 1;

private:
int updateCounter = 0;
float lastPosMessage = 0;
float lastRotMessage = 0;
float lastScaleMessage = 0;

float posInterpolation = 1;
float rotInterpolation = 1;
float scaleInterpolation = 1;

Math::Vector3 targetPos;
Math::Vector3 prevPos;
Math::Quaternion targetRot = Math::Quaternion::identity;
Math::Quaternion prevRot = Math::Quaternion::identity;
Math::Vector3 targetScale;
Math::Vector3 prevScale;

static bool registeredCallbacks;
static std::unordered_map<int, float> posUpdateTimes;
static std::unordered_map<int, float> rotUpdateTimes;
static std::unordered_map<int, float> scaleUpdateTimes;
class NetworkId* netId;
friend class NetworkTransform;
END_COMPONENT(NetworkTransform, Component)

RPC_MESSAGE_DEFINE(PositionMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());

  serialize_float(stream, updateTime);

  serialize_float(stream, localPos.x);
  serialize_float(stream, localPos.y);
  serialize_float(stream, localPos.z);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const PositionMessage* message =
      reinterpret_cast<const PositionMessage*>(otherMessage);

  netId = message->netId;
  updateTime = message->updateTime;
  localPos = message->localPos;
}

public:
int netId = 0;
float updateTime = 0;
Math::Vector3 localPos;
RPC_MESSAGE_FINISH

RPC_MESSAGE_DEFINE(RotationMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());

  serialize_float(stream, updateTime);

  serialize_float(stream, localRot.x);
  serialize_float(stream, localRot.y);
  serialize_float(stream, localRot.z);
  serialize_float(stream, localRot.w);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const RotationMessage* message =
      reinterpret_cast<const RotationMessage*>(otherMessage);

  netId = message->netId;
  updateTime = message->updateTime;
  localRot = message->localRot;
}

public:
int netId = 0;
float updateTime = 0;
Math::Quaternion localRot;
RPC_MESSAGE_FINISH

RPC_MESSAGE_DEFINE(ScaleMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());

  serialize_float(stream, updateTime);

  serialize_float(stream, localScale.x);
  serialize_float(stream, localScale.y);
  serialize_float(stream, localScale.z);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const ScaleMessage* message =
      reinterpret_cast<const ScaleMessage*>(otherMessage);

  netId = message->netId;
  updateTime = message->updateTime;
  localScale = message->localScale;
}

public:
int netId = 0;
float updateTime = 0;
Math::Vector3 localScale;
RPC_MESSAGE_FINISH

}  // namespace Isetta
