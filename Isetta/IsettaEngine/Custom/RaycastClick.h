/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(RaycastClick, Component, true)
private:
 bool useRaycastAll = false;
 float maxRayDistance = 0;

public:
 RaycastClick(bool raycastAll = false, float maxDistance = 0);
 void OnEnable() override;
END_COMPONENT(RaycastClick, Component)
}  // namespace Isetta
