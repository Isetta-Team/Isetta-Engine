/*
 * Copyright (c) 2018 Isetta
 */
#include "Level1.h"
#include "Audio/AudioListener.h"
#include "CameraController.h"
#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Core/Config/Config.h"
#include "Custom/EscapeExit.h"
#include "GameManager.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/Font.h"
#include "Graphics/LightComponent.h"
#include "PlayerController.h"
#include "Scene/Entity.h"

namespace Isetta {

void Level1::OnLevelLoad() {
  Font::AddFontFromFile("Fonts/CONSOLA.TTF", 13.0f);

  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraController>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->AddComponent<AudioListener>();

  Entity* lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600},
                            Math::Vector3{-30, 0, 0});

  Entity* player{Entity::Instantiate("Player")};
  player->SetTransform(Math::Vector3{0, 0, 0}, Math::Vector3{0, 90, 0},
                       0.03f * Math::Vector3::one);
  MeshComponent* playerMesh =
      player->AddComponent<MeshComponent>("Soldier/Soldier.scene.xml");
  player->AddComponent<PlayerController>();

  AnimationComponent* ani =
      player->AddComponent<AnimationComponent>(playerMesh);
  ani->AddAnimation("Soldier/Soldier_Idle.anim", 0, "", false);
  ani->AddAnimation("Soldier/Soldier.anim", 0, "", false);

  Entity* ground{Entity::Instantiate("Ground")};
  ground->AddComponent<MeshComponent>("Ground/Level.scene.xml");

  Entity* gameManager{Entity::Instantiate("Game Manager")};
  gameManager->AddComponent<GameManager>();
  gameManager->AddComponent<EscapeExit>();
  gameManager->AddComponent<EditorComponent>();

  // for (int i = 0; i < 10; i++) {
  // Entity* zombie {AddEntity("Zombie")};
  // zombie->SetTransform(Math::Vector3::forward, Math::Vector3::zero,
  // Math::Vector3::one * 0.01f); MeshComponent* mesh =
  // zombie->AddComponent<MeshComponent>(true, "Zombie/Zombie.scene.xml");
  // AnimationComponent* animation =
  // zombie->AddComponent<AnimationComponent>(true, mesh);
  // animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
  // zombie->AddComponent<Zombie>();
  // }
}
}  // namespace Isetta