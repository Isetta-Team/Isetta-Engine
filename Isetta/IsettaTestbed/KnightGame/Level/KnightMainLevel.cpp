/*
 * Copyright (c) 2018 Isetta
 */
#include "KnightMainLevel.h"

#include "Components/GridComponent.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Graphics/MeshComponent.h"
#include "Scene/Entity.h"
#include "Scene/Primitive.h"

#include "KnightGame/Constants.h"
#include "KnightGame/Gameplay/FollowComponent.h"
#include "KnightGame/Gameplay/KnightController.h"
#include "KnightGame/Gameplay/SpinAttack.h"
#include "KnightGame/Gameplay/SwordController.h"

using namespace Isetta;

namespace KnightGame {
using LightProperty = LightComponent::Property;

void KnightMainLevel::OnLevelLoad() {
  Logger::channelMask.set(static_cast<int>(Debug::Channel::Memory), false);

  Entity* cameraEntity = Entity::Instantiate("Camera");
  CameraComponent* camComp = cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  Entity* lightEntity = Entity::Instantiate("Light");
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* editor{Entity::Instantiate("Editor")};
  editor->AddComponent<GridComponent>();

  Entity* knight = Entity::Instantiate("Knight");
  MeshComponent* mesh =
      knight->AddComponent<MeshComponent>(KNIGHT_PATH + "idle.scene.xml");
  AnimationComponent* animation =
      knight->AddComponent<AnimationComponent>(mesh);
  knight->AddComponent<KnightController>();
  knight->AddComponent<SpinAttack>();

  bool follow[3] = {true, false, false};
  cameraEntity->AddComponent<FollowComponent>(knight, follow);

  Entity* sword = Entity::Instantiate("Sword");
  sword->transform->SetParent(knight->transform);
  sword->AddComponent<MeshComponent>(KNIGHT_PATH + "sword.scene.xml");
  sword->AddComponent<SwordController>(mesh);
}
}  // namespace KnightGame
