/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/AILevel/AITestComponent.h"
#include "AITestAgent.h"
#include "Core/Math/Random.h"
#include "Core/IsettaCore.h"
#include "Input/Input.h"

Isetta::AITestComponent::AITestComponent(const Math::Rect& gridSurface,
                                         const Math::Vector2Int& divideNums,
                                         Transform* tracking)
    : navPlane(gridSurface, divideNums), trackingEntity(tracking) {}

void Isetta::AITestComponent::Update() {
  navPlane.UpdateRoute();
#ifdef _EDITOR
  navPlane.DebugDisplay();
#endif
}

void Isetta::AITestComponent::Awake() {
  Input::RegisterKeyPressCallback(KeyCode::J, [&]() {
    auto entity = Entity::Instantiate("Agent");
    entity->AddComponent<AITestAgent>(&navPlane);
    float randomX = Math::Random::GetRandom01() * 10;
    float randomY = Math::Random::GetRandom01() * 10;
    entity->SetTransform({randomX, 0.0, randomY}, {0, 0, 0}, {0.1, 0.1, 0.1});
  });

  Input::RegisterKeyPressCallback(KeyCode::K, [&]() {
    for (int i = 0; i < 100; ++i) {
      auto entity = Entity::Instantiate("Agent");
      entity->AddComponent<AITestAgent>(&navPlane);
      float randomX = Math::Random::GetRandom01() * 10;
      float randomY = Math::Random::GetRandom01() * 10;
      entity->SetTransform({randomX, 0.0, randomY}, {0, 0, 0}, {0.1, 0.1, 0.1});
    }
  });
  auto zero = Entity::Instantiate("Zero entity");
  zero->SetTransform({0.3, 0, 0.3});
  navPlane.AddTarget(zero->transform);
  navPlane.AddTarget(trackingEntity);

  Input::RegisterKeyPressCallback(KeyCode::P, [&, zero]() {
    navPlane.RemoveTarget(zero->transform);
  });
}
