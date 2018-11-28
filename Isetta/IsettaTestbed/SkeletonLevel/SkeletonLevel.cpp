/*
 * Copyright (c) 2018 Isetta
 */
#include "SkeletonLevel.h"

#include "Components/Editor/EditorComponent.h"
#include "Components/Editor/FrameReporter.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Core/Config/Config.h"
#include "Core/IsettaCore.h"
#include "Events/Events.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"

namespace Isetta {

void SkeletonLevel::OnLevelLoad() {
  Entity *cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();

  Entity *lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity *grid{Entity::Instantiate("Grid")};
  grid->AddComponent<GridComponent>();
  grid->AddComponent<EditorComponent>();

  Entity *zombie{Entity::Instantiate("Zombie")};
  MeshComponent *mesh =
      zombie->AddComponent<MeshComponent>("Zombie/Zombie.scene.xml");
  mesh->SetActive(false);
  AnimationComponent *animation =
      zombie->AddComponent<AnimationComponent>(mesh);
  animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
  zombie->SetTransform(Math::Vector3::zero, Math::Vector3::zero,
                       Math::Vector3::one * 0.01f);
  Entity *zombie2{Entity::Instantiate("Zombie")};
  MeshComponent *mesh2 =
      zombie2->AddComponent<MeshComponent>("Zombie/Zombie.scene.xml");
  AnimationComponent *animation2 =
      zombie2->AddComponent<AnimationComponent>(mesh2);
  animation2->AddAnimation("Zombie/Zombie.anim", 0, "", false);
  zombie2->SetTransform({1, 0, 0}, Math::Vector3::zero,
                        Math::Vector3::one * 0.01f);

  Entity *head{Entity::Instantiate("Head")};
  head->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  head->transform->SetParent(zombie->transform);
  head->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *hips{Entity::Instantiate("Hips")};
  hips->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  hips->transform->SetParent(zombie->transform);
  hips->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *leftShoulder{Entity::Instantiate("LeftShoulder")};
  leftShoulder->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  leftShoulder->transform->SetParent(zombie->transform);
  leftShoulder->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *rightShoulder{Entity::Instantiate("RightShoulder")};
  rightShoulder->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  rightShoulder->transform->SetParent(zombie->transform);
  rightShoulder->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *leftHand{Entity::Instantiate("LeftHand")};
  leftHand->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  leftHand->transform->SetParent(zombie->transform);
  leftHand->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *rightHand{Entity::Instantiate("RightHand")};
  rightHand->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  rightHand->transform->SetParent(zombie->transform);
  rightHand->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *leftFoot{Entity::Instantiate("LeftFoot")};
  leftFoot->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  leftFoot->transform->SetParent(zombie->transform);
  leftFoot->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *rightFoot{Entity::Instantiate("RightFoot")};
  rightFoot->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  rightFoot->transform->SetParent(zombie->transform);
  rightFoot->transform->SetWorldScale({0.1, 0.1, 0.1});

  Events::Instance().RegisterEventListener(
      "CustomUpdate", [=](const EventObject &object) {
        auto positionFunc = [](MeshComponent *mesh, Entity *jointEntity) {
          auto [pos, rot] =
              mesh->GetJointWorldTransform(jointEntity->GetName());
          jointEntity->transform->SetWorldPos(pos);
          jointEntity->transform->SetWorldRot(rot);
        };
        positionFunc(mesh, head);
        positionFunc(mesh, hips);
        positionFunc(mesh, leftShoulder);
        positionFunc(mesh, rightShoulder);
        positionFunc(mesh, leftHand);
        positionFunc(mesh, rightHand);
        positionFunc(mesh, leftFoot);
        positionFunc(mesh, rightFoot);

        Events::Instance().RaiseQueuedEvent(
            EventObject("CustomUpdate", Time::GetTimeFrame() + 1,
                        EventPriority::MEDIUM, {}));
      });
  Events::Instance().RaiseQueuedEvent(EventObject(
      "CustomUpdate", Time::GetTimeFrame() + 1, EventPriority::MEDIUM, {}));
  // cube->transform->SetLocalPos(2.f * Math::Vector3::up);
}
}  // namespace Isetta