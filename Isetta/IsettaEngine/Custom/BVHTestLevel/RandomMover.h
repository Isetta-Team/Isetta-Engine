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

float speed{5.f};
float coolDown{0.f};
float interval{1.f};
float range{0.f};

private:
Math::Vector3 velocity;


END_COMPONENT(RandomMover, Component)
}  // namespace Isetta
