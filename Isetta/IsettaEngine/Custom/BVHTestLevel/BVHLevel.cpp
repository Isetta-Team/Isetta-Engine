/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/BVHTestLevel/BVHLevel.h"
#include "Collisions/CollisionHandler.h"
#include "Collisions/SphereCollider.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Core/Config/Config.h"
#include "Core/DataStructures/Array.h"
#include "Core/Math/Random.h"
#include "Custom/BVHTestLevel/RandomMover.h"
#include "Custom/DebugCollision.h"
#include "Custom/FrameReporter.h"
#include "Custom/IsettaCore.h"
#include "Custom/RaycastClick.h"

#include "Components/Editor/EditorComponent.h"
#include "Custom/LoadNextLevel.h"

namespace Isetta {

using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void BVHLevel::LoadLevel() {
  Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                  []() { Application::Exit(); });

  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent, true>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);
  lightComp->SetProperty<LightProperty::RADIUS>(2500);
  lightComp->SetProperty<LightProperty::FOV>(180);
  lightComp->SetProperty<LightProperty::COLOR>(Color::white);
  lightComp->SetProperty<LightProperty::COLOR_MULTIPLIER>(1.0f);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_COUNT>(1);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_BIAS>(0.01f);

  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  cameraEntity->AddComponent<LoadNextLevel>("EmptyLevel");

  Entity* debug{ADD_ENTITY("Debug")};
  debug->AddComponent<GridComponent>();
  debug->AddComponent<EditorComponent>();
  debug->AddComponent<FrameReporter>();
  debug->AddComponent<RaycastClick>();

  BVTree::drawDebugBoxes = true;

  static bool enable = true;
  Input::RegisterKeyPressCallback(KeyCode::SPACE, [&]() {
    enable = !enable;
    for (RandomMover* randomMover : randomMovers) {
      randomMover->SetActive(enable);
    }
  });

  Input::RegisterKeyPressCallback(KeyCode::KP_ENTER, [&]() {
    BVTree::drawDebugBoxes = !BVTree::drawDebugBoxes;
  });

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

  Input::RegisterKeyPressCallback(KeyCode::KP_5, [&]() {
    for (int i = 0; i < 100; i++) {
      count++;
      Entity* sphere{ADD_ENTITY(Util::StrFormat("Sphere (%d)", count))};
      randomMovers.PushBack(sphere->AddComponent<RandomMover>());
      randomMovers.Back()->SetActive(enable);
      randomMovers.Back()->range = range;
      SphereCollider* col = sphere->AddComponent<SphereCollider>();
      sphere->AddComponent<CollisionHandler>();
      sphere->AddComponent<DebugCollision>();
      const float size = 20;
      sphere->SetTransform(size *
                           Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                         Math::Random::GetRandom01(),
                                         Math::Random::GetRandom01() - 0.5f});
    }
  });

  Input::RegisterKeyPressCallback(KeyCode::KP_6, [&]() {
    count++;
    Entity* sphere{ADD_ENTITY(Util::StrFormat("Sphere (%d)", count))};
    randomMovers.PushBack(sphere->AddComponent<RandomMover>());
    randomMovers.Back()->SetActive(enable);
    randomMovers.Back()->range = range;
    SphereCollider* col = sphere->AddComponent<SphereCollider>();
    sphere->AddComponent<CollisionHandler>();
    sphere->AddComponent<DebugCollision>();
    const float size = 20;
    sphere->SetTransform(size *
                         Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                       Math::Random::GetRandom01(),
                                       Math::Random::GetRandom01() - 0.5f});
    spheres.push(sphere);
  });

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
