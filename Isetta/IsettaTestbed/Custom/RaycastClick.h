/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Raycast into the level on mouse click, debug drawing ray and hit
 * points
 *
 */
namespace Isetta {
DEFINE_COMPONENT(RaycastClick, Component, false)
private:
bool useRaycastAll = false;
float maxRayDistance = 0;

void Raycast(const class Ray& ray);
void RaycastAll(const class Ray& ray);

public:
RaycastClick(bool raycastAll = false, float maxDistance = 0);
void OnEnable() override;
DEFINE_COMPONENT_END(RaycastClick, Component)
}  // namespace Isetta
