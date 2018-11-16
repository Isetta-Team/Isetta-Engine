/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "AI/Nav2DAgent.h"
#include "AI/Nav2DPlane.h"
#include "Core/Time/Time.h"
#include "Scene/Component.h"
#include "Scene/Transform.h"
#include "Core/Debug/DebugDraw.h"

BEGIN_COMPONENT(AITestAgent, Isetta::Component, true)
Isetta::Nav2DAgent agent;

public:
AITestAgent(Isetta::Nav2DPlane* nav2DPlane) : agent(nav2DPlane, 2, 2, 0.2, 1) {}
void Update() override {
  Isetta::Math::Vector3 currPos{GetTransform()->GetWorldPos()};
  auto v =
      agent.GetAIMovement({currPos.x, currPos.z}, Isetta::Time::GetDeltaTime());
  currPos.x += v.x * Isetta::Time::GetDeltaTime();
  currPos.z += v.y * Isetta::Time::GetDeltaTime();
  GetTransform()->SetWorldPos(currPos);
  Isetta::DebugDraw::Cube(GetTransform()->GetLocalToWorldMatrix(), Isetta::Color::green);
}

END_COMPONENT(AITestAgent, Isetta::Component)
