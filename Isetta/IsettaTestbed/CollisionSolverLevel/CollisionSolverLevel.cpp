/*
 * Copyright (c) 2018 Isetta
 */
#include "CollisionSolverLevel.h"

#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Custom/DebugCollision.h"
#include "Custom/KeyTransform.h"
#include "Custom/OscillateMove.h"
#include "Custom/RaycastClick.h"

#include "Components/Editor/FrameReporter.h"

void CollisionSolverLevel::Load() {
  // Camera
  Entity *cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  // We have a specific position we want the camera to start in so that
  // we can see all of the information in the level that we need.
  cameraEntity->SetTransform(Math::Vector3{0, 10, 6}, Math::Vector3{-45, 0, 0},
                             Math::Vector3::one);
  // Flying around for a closer look would be good too (but we don't
  // want it to start disabled).
  cameraEntity->AddComponent<FlyController>(false);

  // Editor component
  // Very helpful for debug information
  Entity *editor{Entity::Instantiate("Editor")};
  editor->AddComponent<EditorComponent>();
  editor->AddComponent<FrameReporter>();
  editor->AddComponent<RaycastClick>();

  // Grid component
  Entity *grid{Entity::Instantiate("Grid")};
  grid->AddComponent<GridComponent>();

  // STATIC - These colliders will be on static entities
  const int COLLIDERS = 3;
  Entity *staticCol[COLLIDERS];

  // To instantiate a static entity, we need to set the third parameter
  // of the Instantiate function to true
  staticCol[0] = Entity::Instantiate("box-collider", nullptr, true);
  staticCol[0]->SetTransform(Math::Vector3{0, 1, 0}, Math::Vector3{0, 0, 0});
  // Add our box collider
  staticCol[0]->AddComponent<BoxCollider>();
  staticCol[0]->AddComponent<CollisionHandler>();
  staticCol[0]->AddComponent<DebugCollision>();  // This will let us know when
                                                 // collisions occur

  // Static sphere collider
  staticCol[1] = Entity::Instantiate("sphere-collider", nullptr, true);
  staticCol[1]->SetTransform(Math::Vector3{0, 1, -4});
  staticCol[1]->AddComponent<SphereCollider>();
  staticCol[1]->AddComponent<CollisionHandler>();
  staticCol[1]->AddComponent<DebugCollision>();

  // Static capsule collider
  staticCol[2] = Entity::Instantiate("capsule-collider", nullptr, true);
  staticCol[2]->SetTransform(Math::Vector3{0, 1, -8});
  // Capsule colliders typically need a lot more inputs when creating their
  // components to correctly set the orientation
  staticCol[2]->AddComponent<CapsuleCollider>(
      false, Math::Vector3::zero, 0.5, 2, CapsuleCollider::Direction::X_AXIS);
  staticCol[2]->AddComponent<CollisionHandler>();
  staticCol[2]->AddComponent<DebugCollision>();

  // DYNAMIC - These colliders will be movable throughout the scene
  box = Entity::Instantiate("box-collider-dynamic");
  box->SetTransform(Math::Vector3{3, 1, 0}, Math::Vector3{0, 0, 0});
  box->AddComponent<BoxCollider>();
  box->AddComponent<KeyTransform>();  // We want to be able to move this
                                      // collider using our keyboard

  sphere = Entity::Instantiate("sphere-collider-dynamic");
  sphere->SetTransform(Math::Vector3{3, 1, -4});
  sphere->AddComponent<SphereCollider>();
  // This _can_ be moved by keys, but we don't want to move all of the colliders
  // at the same time!
  sphere->AddComponent<KeyTransform>()->SetActive(false);

  capsule = Entity::Instantiate("capsule-collider-dynamic");
  capsule->transform->SetLocalPos(Math::Vector3{3, 1, -8});
  capsule->transform->SetLocalRot(-30 * Math::Vector3::up);
  capsule->AddComponent<CapsuleCollider>(0.5, 2,
                                         CapsuleCollider::Direction::X_AXIS);
  capsule->AddComponent<KeyTransform>()->SetActive(false);

  // Manually registering the Escape key to exit the application
  Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                  []() { Application::Exit(); });

  // The 1, 2, and 3 keys control which of the colliders we can move
  // using our keyboard
  Input::RegisterKeyPressCallback(KeyCode::NUM1, [&]() {
    box->GetComponent<KeyTransform>()->SetActive(true);
    sphere->GetComponent<KeyTransform>()->SetActive(false);
    capsule->GetComponent<KeyTransform>()->SetActive(false);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM2, [&]() {
    box->GetComponent<KeyTransform>()->SetActive(false);
    sphere->GetComponent<KeyTransform>()->SetActive(true);
    capsule->GetComponent<KeyTransform>()->SetActive(false);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM3, [&]() {
    box->GetComponent<KeyTransform>()->SetActive(false);
    sphere->GetComponent<KeyTransform>()->SetActive(false);
    capsule->GetComponent<KeyTransform>()->SetActive(true);
  });

  // We want to tweak the mass of our sphere collider to test how different
  // masses influence the collision solving
  Input::RegisterKeyPressCallback(
      KeyCode::NUM9, [&]() { sphere->GetComponent<Collider>()->mass -= 10; });
  Input::RegisterKeyPressCallback(
      KeyCode::NUM0, [&]() { sphere->GetComponent<Collider>()->mass += 10; });
}
