/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/InEngineTestLevel.h"
#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Graphics/Texture.h"

namespace Isetta {
using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void InEngineTestLevel::LoadLevel() {
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

  Entity* grid{ADD_ENTITY("Grid")};
  grid->AddComponent<GridComponent>();
  grid->AddComponent<EditorComponent>();

  Input::RegisterKeyPressCallback(KeyCode::PAGE_UP, [&]() {
    static int count = 0;
    Entity* zombie{ADD_ENTITY(Util::StrFormat("Zombie%d", count))};
    zombies.push(zombie);
    AnimationComponent* animation = zombie->AddComponent<AnimationComponent>(
        zombie->AddComponent<MeshComponent>("Zombie/Zombie.scene.xml"));
    animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
    zombie->SetTransform(Math::Vector3::left * count++, Math::Vector3::zero,
                         Math::Vector3::one * 0.01f);
    Entity* cube{ADD_ENTITY(Util::StrFormat("Cube%d", count))};
    cube->AddComponent<MeshComponent>("primitive/cube.scene.xml");
    cube->GetTransform()->SetParent(zombie->GetTransform());
    cube->GetTransform()->SetLocalPos(Math::Vector3::zero);
  });

  Input::RegisterKeyPressCallback(KeyCode::PAGE_DOWN, [&]() {
    if (zombies.empty()) return;
    Entity* zombie = zombies.front();
    zombies.pop();
    Entity::Destroy(zombie);
  });
}
}  // namespace Isetta