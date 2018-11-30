/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
DEFINE_COMPONENT(RaycastClick, Component, true)
private:
 bool useRaycastAll = false;
 float maxRayDistance = 0;

public:
 RaycastClick(bool raycastAll = false, float maxDistance = 0);
 void OnEnable() override;
DEFINE_COMPONENT_END(RaycastClick, Component)
}  // namespace Isetta
