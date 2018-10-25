/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#pragma once
#include "Core/Math/Vector2.h"
#include "ISETTA_API.h"
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(GridComponent, Component)
public:
  void Update() override;
CREATE_COMPONENT_END(GridComponent, Component)
}  // namespace Isetta
