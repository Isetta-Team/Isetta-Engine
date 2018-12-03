/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Move the entity transform in discrete steps or continuous motion
 *  M toggle between step and continuous
 *
 *  RIGHT_ARROW move right
 *  LEFT_ARROW  move left
 *  UP_ARROW    move forward
 *  DOWN_ARROW  move back
 *  PAGE_UP     move up
 *  PAGE_DOWN   move down
 *
 *  NUM1  rotate about X
 *  NUM2  rotate about -X by step
 *  NUM3  rotate about -Y by step
 *  NUM4  rotate about Y by step
 *  NUM5  rotate about Z by step
 *  NUM6  rotate about -Z by step
 *
 *  NUM7  scale by 1.1 uniformly
 *  NUM8  scale by 0.9 uniformly
 *
 */
namespace Isetta {
DEFINE_COMPONENT(KeyTransform, Component, false)
private:
float step;
bool pressed = true;

public:
KeyTransform(float step = 1) : step{step} {}

void OnEnable() override;
void Update() override;
DEFINE_COMPONENT_END(KeyTransform, Component)
}  // namespace Isetta
