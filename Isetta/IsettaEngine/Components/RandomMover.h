/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
BEGIN_COMPONENT(RandomMover, Component, false)
public:
void Update() override;

private:
Math::Vector3 velocity;

float speed{5.f};
float coolDown{0.f};
float interval{3.f};


END_COMPONENT(RandomMover, Component)
}  // namespace Isetta
