/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

/**
 * @brief Moves the entity in a random direction for a given interval
 *
 */
namespace Isetta {
DEFINE_COMPONENT(RandomMover, Component, false)
public:
void Update() override;

float speed{5.f};
float coolDown{0.f};
float interval{1.f};
float range{0.f};

private:
Math::Vector3 velocity;

DEFINE_COMPONENT_END(RandomMover, Component)
}  // namespace Isetta
