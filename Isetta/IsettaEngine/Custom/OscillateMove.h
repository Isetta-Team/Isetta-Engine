/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector3.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(OscillateMove, Component, true)
private:
int axis = 0;
int direction = 1;
float speed = 1;
Math::Vector3 origin = Math::Vector3::zero;
float distance = 5;

public:
OscillateMove(int axis = 0, float speed = 1, int direction = 1,
              float distance = 5,
              const Math::Vector3& origin = Math::Vector3::zero)
    : axis{axis},
      speed{speed},
      direction{direction},
      distance{distance},
      origin{origin} {}

void OnEnable() override;
void Update() override;
END_COMPONENT(OscillateMove, Component)
}  // namespace Isetta
