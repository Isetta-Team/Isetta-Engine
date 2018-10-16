/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Core/Math/Vector3.h"
#include "Audio/AudioSource.h"

namespace Isetta {
class Bullet : public Component {
 public:
  void Initialize(const Math::Vector3& pos, const Math::Vector3& flyDir);
  void OnEnable() override;
  void Update() override;

  // exposed for easier debug inspector
  static float flySpeed;
private:
  float lifeTime{1.5};
  float elapsedTime{0};
  float damage{35};
  Math::Vector3 dir{};
  bool initialized = false;
  AudioSource audio;
};
}  // namespace Isetta
