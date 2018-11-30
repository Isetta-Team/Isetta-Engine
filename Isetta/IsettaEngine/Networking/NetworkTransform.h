/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Config/Config.h"
#include "Core/Math/Math.h"
#include "ISETTA_API.h"
#include "Networking/Messages.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(NetworkTransform, Component, true)
public:
void Start() override;
void Update() override;
void FixedUpdate() override;

void ForceSendTransform(bool snap = false);
void SnapLocalTransform();
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
static std::unordered_map<int, float> serverPosTimestamps;
static std::unordered_map<int, float> serverRotTimestamps;
static std::unordered_map<int, float> serverScaleTimestamps;
class NetworkId* netId;
friend class NetworkTransform;
friend class NetworkManager;
DEFINE_COMPONENT_END(NetworkTransform, Component)

// TODO(Caleb): ParentMessage
DEFINE_NETWORK_MESSAGE(ParentMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());
  serialize_int(stream, parentNetId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const ParentMessage* message = reinterpret_cast<const ParentMessage*>(otherMessage);

  netId = message->netId;
  parentNetId = message->parentNetId;
}

int netId = 0;
int parentNetId = 0;

DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(PositionMessage) template <typename Stream>
  bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());

  serialize_float(stream, timestamp);

  serialize_float(stream, localPos.x);
  serialize_float(stream, localPos.y);
  serialize_float(stream, localPos.z);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const PositionMessage* message =
      reinterpret_cast<const PositionMessage*>(otherMessage);

  netId = message->netId;
  timestamp = message->timestamp;
  localPos = message->localPos;
}

public:
int netId = 0;
float timestamp = 0;
Math::Vector3 localPos;
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(RotationMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());

  serialize_float(stream, timestamp);

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
  timestamp = message->timestamp;
  localRot = message->localRot;
}

public:
int netId = 0;
float timestamp = 0;
Math::Quaternion localRot;
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(ScaleMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());

  serialize_float(stream, timestamp);

  serialize_float(stream, localScale.x);
  serialize_float(stream, localScale.y);
  serialize_float(stream, localScale.z);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const ScaleMessage* message =
      reinterpret_cast<const ScaleMessage*>(otherMessage);

  netId = message->netId;
  timestamp = message->timestamp;
  localScale = message->localScale;
}

public:
int netId = 0;
float timestamp = 0;
Math::Vector3 localScale;
DEFINE_NETWORK_MESSAGE_END

DEFINE_NETWORK_MESSAGE(TransformMessage)
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0,
                Config::Instance().networkConfig.maxNetID.GetVal());

  serialize_float(stream, timestamp);

  serialize_bool(stream, snap);

  serialize_float(stream, localPos.x);
  serialize_float(stream, localPos.y);
  serialize_float(stream, localPos.z);

  serialize_float(stream, localRot.x);
  serialize_float(stream, localRot.y);
  serialize_float(stream, localRot.z);
  serialize_float(stream, localRot.w);

  serialize_float(stream, localScale.x);
  serialize_float(stream, localScale.y);
  serialize_float(stream, localScale.z);
  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const TransformMessage* message =
      reinterpret_cast<const TransformMessage*>(otherMessage);

  netId = message->netId;
  timestamp = message->timestamp;
  snap = message->snap;
  localPos = message->localPos;
  localRot = message->localRot;
  localScale = message->localScale;
}

public:
int netId = 0;
float timestamp = 0;
bool snap = false;
Math::Vector3 localPos;
Math::Quaternion localRot;
Math::Vector3 localScale;
DEFINE_NETWORK_MESSAGE_END

}  // namespace Isetta
