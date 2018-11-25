/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector2.h"
#include "ISETTA_API.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(GridComponent, Component, false)
public:
void Update() override;
END_COMPONENT(GridComponent, Component)
}  // namespace Isetta
