/*
 * Copyright (c) 2018 Isetta
 */
#include "BVHLevel.h"

#include "Components/Editor/FrameReporter.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"

#include "BVHLevel/RandomMover.h"
#include "Components/Editor/EditorComponent.h"
#include "Custom/DebugCollision.h"
#include "Custom/EscapeExit.h"
#include "Custom/RaycastClick.h"
#include "RandomMover.h"

namespace Isetta {

void BVHLevel::Load() {
  Entity* lightEntity = Entity::Instantiate("Light");
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();

  Entity* debug = Entity::Instantiate("Debug");
  debug->AddComponent<GridComponent>();
  debug->AddComponent<EditorComponent>();
  debug->AddComponent<EscapeExit>();
  debug->AddComponent<FrameReporter>();
  debug->AddComponent<RaycastClick>(true);

  Config::Instance().drawConfig.bvtDrawAABBs.SetVal("1");

  // Enable/Disable the random movement of all entities
  static bool enable = true;
  Input::RegisterKeyPressCallback(KeyCode::SPACE, [&]() {
    enable = !enable;
    for (RandomMover* randomMover : randomMovers) {
      randomMover->SetActive(enable);
    }
  });

  // Draw BVTree AABB boxes
  Input::RegisterKeyPressCallback(KeyCode::KP_ENTER, [&]() {
    Config::Instance().drawConfig.bvtDrawAABBs.SetVal(
        Config::Instance().drawConfig.bvtDrawAABBs.GetVal() ? "0" : "1");
  });

  // Decrease/Increase the range the entities move with random walks
  static float range = 10;
  Input::RegisterKeyPressCallback(KeyCode::KP_7, [&]() {
    --range;
    for (RandomMover* randomMover : randomMovers) {
      randomMover->range = range;
      randomMover->coolDown = 0;
    }
  });
  Input::RegisterKeyPressCallback(KeyCode::KP_9, [&]() {
    ++range;
    for (RandomMover* randomMover : randomMovers) {
      randomMover->range = range;
      randomMover->coolDown = 0;
    }
  });

  // Instantiate 100 sphere collider entities with random walk in level
  Input::RegisterKeyPressCallback(KeyCode::KP_5, [&]() {
    for (int i = 0; i < 100; ++i) {
      ++count;
      Entity* sphere{
          Entity::Instantiate(Util::StrFormat("Sphere (%d)", count))};
      randomMovers.PushBack(sphere->AddComponent<RandomMover>());
      randomMovers.Back()->SetActive(enable);
      randomMovers.Back()->range = range;
      sphere->AddComponent<SphereCollider>();
      sphere->AddComponent<CollisionHandler>();
      // Changes color of collider on collision
      sphere->AddComponent<DebugCollision>();
      const float size = 20;
      sphere->SetTransform(size *
                           Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                         Math::Random::GetRandom01(),
                                         Math::Random::GetRandom01() - 0.5f});
    }
  });

  // Instantiate 1 sphere collider entity with random walk in level
  Input::RegisterKeyPressCallback(KeyCode::KP_6, [&]() {
    ++count;
    Entity* sphere{Entity::Instantiate(Util::StrFormat("Sphere (%d)", count))};
    randomMovers.PushBack(sphere->AddComponent<RandomMover>());
    randomMovers.Back()->SetActive(enable);
    randomMovers.Back()->range = range;
    sphere->AddComponent<SphereCollider>();
    sphere->AddComponent<CollisionHandler>();
    sphere->AddComponent<DebugCollision>();
    const float size = 20;
    sphere->SetTransform(size *
                         Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                       Math::Random::GetRandom01(),
                                       Math::Random::GetRandom01() - 0.5f});
    spheres.push(sphere);
  });

  // Remove a single collider entity from the level
  Input::RegisterKeyPressCallback(KeyCode::KP_4, [&]() {
    if (!spheres.empty()) {
      count--;
      Entity* sphere = spheres.front();
      spheres.pop();
      Entity::Destroy(sphere);
    }
  });
}
}  // namespace Isetta
