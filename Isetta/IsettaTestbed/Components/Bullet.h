/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
class Bullet : public Component {
 public:
  void Initialize(const Math::Vector3& pos, const Math::Vector3& flyDir);
  void OnEnable() override;
  void Update() override;

  // exposed for easier debug inspector
  static float flySpeed;
  float lifeTime{5};
  float elapsedTime{0};
  Math::Vector3 dir{};
  
};
}  // namespace Isetta
