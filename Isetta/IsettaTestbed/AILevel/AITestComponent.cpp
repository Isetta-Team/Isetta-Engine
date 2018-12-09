/*
 * Copyright (c) 2018 Isetta
 */
#include "AITestComponent.h"

#include "AITestAgent.h"

Isetta::AITestComponent::AITestComponent(const Math::Rect& gridSurface,
                                         const Math::Vector2Int& divideNums,
                                         Transform* tracking)
    : navPlane(gridSurface, divideNums), trackingEntity(tracking) {}

void Isetta::AITestComponent::Update() {
  // The UpdateRoute function updates the internal state of the navigation
  // plane, based on its targets' position
  navPlane.UpdateRoute();
#ifdef _EDITOR
  navPlane.DebugDraw();
#endif
}

void Isetta::AITestComponent::Awake() {
  Input::RegisterKeyPressCallback(KeyCode::J, [&]() {
    auto entity = Entity::Instantiate("Agent");
    // See comment in test agent
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
  // Add targets to the navigation plane
  navPlane.AddTarget(zero->transform);
  navPlane.AddTarget(trackingEntity);

  Input::RegisterKeyPressCallback(
    // And remove it
      KeyCode::P, [&, zero]() { navPlane.RemoveTarget(zero->transform); });
}
