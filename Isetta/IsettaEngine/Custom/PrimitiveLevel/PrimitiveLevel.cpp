/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/PrimitiveLevel/PrimitiveLevel.h"

#include "Core/Config/Config.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"

#include "Components/FlyController.h"
#include "Custom/EscapeExit.h"
#include "Scene/Primitive.h"

namespace Isetta {
void PrimitiveLevel::OnLevelLoad() {
  Entity* cameraEntity{Entity::CreateEntity("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();
  cameraEntity->AddComponent<EscapeExit>();

  Entity* lightEntity{Entity::CreateEntity("Light")};
  lightEntity->AddComponent<LightComponent>("materials/light.material.xml",
                                            "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* entity;
  entity = Primitive::Create(Primitive::Type::Capsule);
  entity->transform->SetWorldPos(Math::Vector3{0, 1.f, 0});
  entity = Primitive::Create(Primitive::Type::Cube);
  entity->transform->SetWorldPos(Math::Vector3{1.5f, 0.5f, 0});
  entity = Primitive::Create(Primitive::Type::Cylinder);
  entity->transform->SetWorldPos(Math::Vector3{-1.5f, 1.f, 0});
  entity = Primitive::Create(Primitive::Type::Grid, true);
  entity = Primitive::Create(Primitive::Type::Quad);
  entity->transform->SetWorldPos(Math::Vector3{1.5f, 0.5f, 1});
  entity->transform->SetWorldRot(Math::Vector3{0, 180.f, 0});
  entity = Primitive::Create(Primitive::Type::Sphere, true);
  entity->transform->SetWorldPos(Math::Vector3{1.5f, 2.f, 0});
}
}  // namespace Isetta