/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "AI/Nav2DAgent.h"
#include "AI/Nav2DPlane.h"
#include "Core/DataStructures/Array.h"

namespace Isetta {
BEGIN_COMPONENT(AITestComponent, Component, true)
// A Nav2DPlane is a representation of a navigatable 2D plane
Nav2DPlane navPlane;

public:
// The grid surface is the size of the navigation plane, the divide number is
// how the plane is divided into a grid (10x10 or 5x5)
AITestComponent(const Math::Rect& gridSurface,
                const Math::Vector2Int& divideNums, Transform* tracking);
// void GuiUpdate() override;
void Update() override;
void Awake() override;
Transform* trackingEntity;
Array<Nav2DAgent*> agents;
END_COMPONENT(AITestComponent, Component)
}  // namespace Isetta
