/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/DataStructures/Array.h"
#include "AI/Nav2DPlane.h"
#include "Scene/Component.h"
#include "Scene/Transform.h"
#include "AI/Nav2DAgent.h"

namespace Isetta {
BEGIN_COMPONENT(AITestComponent, Component, true)
Nav2DPlane navPlane;

public:
AITestComponent(const Math::Rect& gridSurface,
                const Math::Vector2Int& divideNums, Transform* tracking);
// void GuiUpdate() override;
void Update() override;
void Awake() override;
Transform* trackingEntity;
Array<Nav2DAgent*> agents;
END_COMPONENT(AITestComponent, Component)
}  // namespace Isetta
