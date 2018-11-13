/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EmptyLevel/EmptyLevel.h"
#include "Application.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Core/DataStructures/Array.h"
#include "Custom/EscapeExit.h"

#include <vector>

namespace Isetta {

using CameraProperty = CameraComponent::Property;

void EmptyLevel::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  cameraEntity->AddComponent<EscapeExit>();

  Array<std::string> arr;
  arr.PushBack("test");
  arr.PushBack("test");
  arr.~Array();

  // struct TestStruct {
  //  // std::vector<int> test;
  //  uint8_t test[5];
  //};
  // Array<TestStruct> arr;
  // arr.PushBack(TestStruct{});
  // arr.PushBack(TestStruct{});
  // arr.PushBack(TestStruct{});
  // arr.PushBack(TestStruct{});
  // arr.PushBack(TestStruct{});

  Application::Exit();

  // Array<int> arr;
  // arr.PushBack(1);
  // arr.PushBack(1);
}
}  // namespace Isetta