/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector2.h"
#include "ISETTA_API.h"
#include "Scene/Component.h"

namespace Isetta {
/**
 * @brief Controls an object with typical first person shooter controls and
 * slight variations
 *
 *  F: Toggle enabling the mouse to affect look direction
 *  CTRL + KP_0: Reset entity to origin
 *  SCROLL: Up scroll increases speed/down scroll speed with SHIFT
 *  LEFT_SHIFT: increases speed of movement
 *  W: move in local forward direction
 *  A: move in local left direction
 *  S: move in local back direction
 *  D: move in local right direction
 *  Q: move in local down direction
 *  E: move in local up direction
 */
DEFINE_COMPONENT(FlyController, Component, false)
public:
FlyController() = default;
/**
 * @brief Construct a new Fly Controller object
 *
 * @param enableLook with mouse (moving mouse control look direction)
 */
FlyController(bool enableLook);

void OnEnable() override;
void Update() override;

private:
Math::Vector2 lastFrameMousePos;
float rotX{};
float rotY{};
float lookRotationSpeed{-0.1f};
float flySpeed = 15.f;
float flySpeedMultiplier = 2.0f;
bool enableLook = true;
DEFINE_COMPONENT_END(FlyController, Component)
}  // namespace Isetta
