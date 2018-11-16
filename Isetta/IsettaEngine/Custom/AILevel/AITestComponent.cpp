/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/AILevel/AITestComponent.h"
#include "AITestAgent.h"
#include "Custom/IsettaCore.h"
#include "Graphics/ParticleSystemComponent.h"
#include "Input/Input.h"
#include "Core/Math/Random.h"

Isetta::AITestComponent::AITestComponent(const Math::Rect& gridSurface,
                                         const Math::Vector2Int& divideNums)
    : navPlane(gridSurface, divideNums) {}

void Isetta::AITestComponent::Update() {
  navPlane.SetTarget(Math::Vector2{trackingEntity->GetWorldPos().x,
                                   trackingEntity->GetWorldPos().z});
  navPlane.DebugDisplay();
}

void Isetta::AITestComponent::Awake() {
  Input::RegisterKeyPressCallback(KeyCode::J, [&]() {
    auto entity = ADD_ENTITY("Agent");
    entity->AddComponent<AITestAgent>(&navPlane);
    float randomX = Math::Random::GetRandom01() * 10;
    float randomY = Math::Random::GetRandom01() * 10;
    entity->SetTransform({randomX, 0.0, randomY}, {0, 0, 0}, {0.1, 0.1, 0.1});
  });

  Input::RegisterKeyPressCallback(KeyCode::K, [&]() {
    for (int i = 0; i < 100; i++) {
      auto entity = ADD_ENTITY("Agent");
      entity->AddComponent<AITestAgent>(&navPlane);
      float randomX = Math::Random::GetRandom01() * 10;
      float randomY = Math::Random::GetRandom01() * 10;
      entity->SetTransform({randomX, 0.0, randomY}, {0, 0, 0}, {0.1, 0.1, 0.1});
    }
  });
}
