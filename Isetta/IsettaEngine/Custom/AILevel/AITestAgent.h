/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "AI/Nav2DAgent.h"
#include "AI/Nav2DPlane.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Time/Time.h"
#include "Scene/Component.h"
#include "Scene/Transform.h"
#include "Scene/Entity.h"

BEGIN_COMPONENT(AITestAgent, Isetta::Component, true)
Isetta::Nav2DPlane* navPlane;
Isetta::Nav2DAgent* navAgent;

public:
AITestAgent(Isetta::Nav2DPlane* nav2DPlane) : navPlane{nav2DPlane} {}

void Awake() override { navAgent = entity->AddComponent<Isetta::Nav2DAgent>(navPlane); }

void Update() override {
  Isetta::Math::Vector3 currPos{transform->GetWorldPos()};
  auto v =
      navAgent->GetAIMovement({currPos.x, currPos.z}, Isetta::Time::GetDeltaTime());
  currPos.x += v.x * Isetta::Time::GetDeltaTime();
  currPos.z += v.y * Isetta::Time::GetDeltaTime();
  transform->SetWorldPos(currPos);
  Isetta::DebugDraw::Cube(transform->GetLocalToWorldMatrix(),
                          Isetta::Color::green);
}

END_COMPONENT(AITestAgent, Isetta::Component)
