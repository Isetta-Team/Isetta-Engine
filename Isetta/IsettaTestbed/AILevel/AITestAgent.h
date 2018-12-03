/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "AI/Nav2DAgent.h"
#include "AI/Nav2DPlane.h"

DEFINE_COMPONENT(AITestAgent, Isetta::Component, true)
Isetta::Nav2DPlane* navPlane;
Isetta::Nav2DAgent* navAgent;

public:
AITestAgent(Isetta::Nav2DPlane* nav2DPlane) : navPlane{nav2DPlane} {}

// Assign the navigation plane to the navigation agent
void Awake() override { navAgent = entity->AddComponent<Isetta::Nav2DAgent>(navPlane); }

void Update() override {
  Isetta::Math::Vector3 currPos{transform->GetWorldPos()};
  // Get the suggested movement direction from the agent
  auto v =
      navAgent->GetAIMovement({currPos.x, currPos.z}, Isetta::Time::GetDeltaTime());
  currPos.x += v.x * Isetta::Time::GetDeltaTime();
  currPos.z += v.y * Isetta::Time::GetDeltaTime();
  transform->SetWorldPos(currPos);
  Isetta::DebugDraw::Cube(transform->GetLocalToWorldMatrix(),
                          Isetta::Color::green);
}

DEFINE_COMPONENT_END(AITestAgent, Isetta::Component)
