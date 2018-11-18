/*
 * Copyright (c) 2018 Isetta
 */
#include "MeshAnimLevel.h"

#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Core/Config/Config.h"
#include "Custom/EscapeExit.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Components/Editor/FrameReporter.h"

namespace Isetta {
using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void MeshAnimLevel::LoadLevel() {
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

  Entity* grid{ADD_ENTITY("Grid")};
  grid->AddComponent<GridComponent>();
  grid->AddComponent<EditorComponent>();
  grid->AddComponent<EscapeExit>();

  static int count = 0;
  int zombieCount = 100;
  int size = 10;

  for (int i = 0; i < zombieCount; ++i) {
    Entity* zombie{AddEntity(Util::StrFormat("Zombie (%d)", count++))};
    zombies.push(zombie);
    MeshComponent* mesh =
        zombie->AddComponent<MeshComponent>("Zombie/Zombie.scene.xml");
    AnimationComponent* animation =
        zombie->AddComponent<AnimationComponent>(mesh);
    animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
    zombie->SetTransform(Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                       Math::Random::GetRandom01() - 0.5f,
                                       Math::Random::GetRandom01() - 0.5f} *
                             size,
                         Math::Vector3::zero, Math::Vector3::one * 0.01f);
  }

  Input::RegisterKeyPressCallback(KeyCode::UP_ARROW, [=]() {
    for (int i = 0; i < 10; ++i) {
      Entity* zombie{this->AddEntity(Util::StrFormat("Zombie (%d)", count++))};
      this->zombies.push(zombie);
      MeshComponent* mesh =
          zombie->AddComponent<MeshComponent>("Zombie/Zombie.scene.xml");
      AnimationComponent* animation =
          zombie->AddComponent<AnimationComponent>(mesh);
      animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
      zombie->SetTransform(Math::Vector3{Math::Random::GetRandom01() - 0.5f,
                                         Math::Random::GetRandom01() - 0.5f,
                                         Math::Random::GetRandom01() - 0.5f} *
                               size,
                           Math::Vector3::zero, Math::Vector3::one * 0.01f);
    }
  });
  Input::RegisterKeyPressCallback(KeyCode::DOWN_ARROW, [&]() {
    for (int i = 0; i < 10; ++i) {
      if (zombies.empty()) return;
      Entity::Destroy(zombies.front());
      zombies.pop();
    }
  });

  // Entity* cube{AddEntity("Cube")};
  // cube->AddComponent<MeshComponent>("primitive/cube.scene.xml");
  // cube->transform->SetParent(zombie->transform);
  // cube->transform->SetLocalPos(2.f * Math::Vector3::up);

  // Application::Exit();
}
}  // namespace Isetta