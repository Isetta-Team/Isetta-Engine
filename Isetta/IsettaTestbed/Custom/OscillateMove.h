/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Osciallate movement between distance at speed
 *
 */
namespace Isetta {
DEFINE_COMPONENT(OscillateMove, Component, false)
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
DEFINE_COMPONENT_END(OscillateMove, Component)
}  // namespace Isetta
