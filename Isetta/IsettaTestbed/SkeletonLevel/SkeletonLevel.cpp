/*
 * Copyright (c) 2018 Isetta
 */
#include "SkeletonLevel.h"

#include "Components/Editor/EditorComponent.h"
#include "Components/Editor/FrameReporter.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Components/JointFollow.h"

namespace Isetta {

void SkeletonLevel::Load() {
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
      zombie->AddComponent<MeshComponent>("Halves/Zombie/Zombie.scene.xml");
  mesh->SetActive(false);
  AnimationComponent *animation =
      zombie->AddComponent<AnimationComponent>(mesh);
  animation->AddAnimation("Halves/Zombie/Zombie.anim", 0, "", false);
  zombie->SetTransform(Math::Vector3::zero, Math::Vector3::zero,
                       Math::Vector3::one * 0.01f);
  Entity *zombie2{Entity::Instantiate("Zombie")};
  MeshComponent *mesh2 =
      zombie2->AddComponent<MeshComponent>("Halves/Zombie/Zombie.scene.xml");
  AnimationComponent *animation2 =
      zombie2->AddComponent<AnimationComponent>(mesh2);
  animation2->AddAnimation("Halves/Zombie/Zombie.anim", 0, "", false);
  zombie2->SetTransform({1, 0, 0}, Math::Vector3::zero,
                        Math::Vector3::one * 0.01f);

  Entity *head{Primitive::Create(Primitive::Type::Cube, "Head", false)};
  head->transform->SetParent(zombie->transform);
  head->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *hips{Primitive::Create(Primitive::Type::Cube, "Hips", false)};
  hips->transform->SetParent(zombie->transform);
  hips->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *leftShoulder{
      Primitive::Create(Primitive::Type::Cube, "LeftShoulder", false)};
  leftShoulder->transform->SetParent(zombie->transform);
  leftShoulder->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *rightShoulder{
      Primitive::Create(Primitive::Type::Cube, "RightShoulder", false)};
  rightShoulder->transform->SetParent(zombie->transform);
  rightShoulder->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *leftHand{Primitive::Create(Primitive::Type::Cube, "LeftHand", false)};
  leftHand->transform->SetParent(zombie->transform);
  leftHand->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *rightHand{
      Primitive::Create(Primitive::Type::Cube, "RightHand", false)};
  rightHand->transform->SetParent(zombie->transform);
  rightHand->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *leftFoot{Primitive::Create(Primitive::Type::Cube, "LeftFoot", false)};
  leftFoot->transform->SetParent(zombie->transform);
  leftFoot->transform->SetWorldScale({0.1, 0.1, 0.1});
  Entity *rightFoot{
      Primitive::Create(Primitive::Type::Cube, "RightFoot", false)};
  rightFoot->transform->SetParent(zombie->transform);
  rightFoot->transform->SetWorldScale({0.1, 0.1, 0.1});
  rightFoot->AddComponent<JointFollow>(mesh, rightFoot->GetName());

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
        // positionFunc(mesh, rightFoot);

        Events::Instance().RaiseQueuedEvent(
            EventObject("CustomUpdate", Time::GetFrameCount() + 1,
                        EventPriority::MEDIUM, {}));
      });
  Events::Instance().RaiseQueuedEvent(EventObject(
      "CustomUpdate", Time::GetFrameCount() + 1, EventPriority::MEDIUM, {}));
  // cube->transform->SetLocalPos(2.f * Math::Vector3::up);
}
}  // namespace Isetta