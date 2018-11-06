/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/Week10Level.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"

using namespace Isetta;
using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void Week10Level::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 0.25f, 3.0f});
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent, true>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 6, 3.5f}, Math::Vector3{-30, 0, 0},
                            Math::Vector3::one);
  lightComp->SetProperty<LightProperty::RADIUS>(2500);
  lightComp->SetProperty<LightProperty::FOV>(180);
  lightComp->SetProperty<LightProperty::COLOR>(Color::white);
  lightComp->SetProperty<LightProperty::COLOR_MULTIPLIER>(1.0f);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_COUNT>(1);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_BIAS>(0.01f);

  Entity* debug{ADD_ENTITY("Debug")};
  debug->AddComponent<GridComponent>();
  Entity* ground{ADD_ENTITY("Ground")};
  ground->AddComponent<MeshComponent>("primitive/cube.scene.xml");
  ground->SetTransform(Math::Vector3{0, -5, 0}, Math::Vector3::zero,
                       Math::Vector3{6.0f, 10.0f, 1});
}
