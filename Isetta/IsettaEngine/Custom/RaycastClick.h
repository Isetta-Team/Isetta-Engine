/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(RaycastClick, Component, true)
private:
 bool useRaycastAll = false;

public:
 RaycastClick(bool raycastAll = false);
 void OnEnable() override;
END_COMPONENT(RaycastClick, Component)
}  // namespace Isetta
