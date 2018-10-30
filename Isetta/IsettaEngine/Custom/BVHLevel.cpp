/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/BVHLevel.h"
#include "Collisions/SphereCollider.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Components/RandomMover.h"
#include "Core/Config/Config.h"
#include "Core/Math/Random.h"
#include "FrameReporter.h"
#include "IsettaCore.h"

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
  cameraEntity->AddComponent<FlyController, true>();
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  Entity* debug{ADD_ENTITY("Debug")};
  debug->AddComponent<GridComponent>();
  debug->AddComponent<FrameReporter>();
  Input::RegisterKeyPressCallback(KeyCode::KP_5, []() {
    for (int i = 0; i < 100; i++) {
      static int count = 0;
      count++;
      Entity* sphere{ADD_ENTITY(Util::StrFormat("Sphere (%d)", count))};
      sphere->AddComponent<RandomMover>();
      auto col = sphere->AddComponent<SphereCollider>();
      const float size = 20;
      sphere->SetTransform(size *
                           Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                         Math::Random::GetRandom01(),
                                         Math::Random::GetRandom01() - 0.5f});
      col->AddToBVTree();
    }
  });
}
}  // namespace Isetta
