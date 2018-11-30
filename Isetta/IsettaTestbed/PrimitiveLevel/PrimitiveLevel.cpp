/*
 * Copyright (c) 2018 Isetta
 */
#include "PrimitiveLevel.h"

#include "Components/FlyController.h"
#include "Custom/EscapeExit.h"

namespace Isetta {
void PrimitiveLevel::Load() {
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();
  cameraEntity->AddComponent<EscapeExit>();

  Entity* lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* entity;
  // Create capsule mesh entity, no collider
  entity = Primitive::Create(Primitive::Type::Capsule);
  entity->transform->SetWorldPos(Math::Vector3{0, 1.f, 0});
  // Create cube mesh entity, no collider
  entity = Primitive::Create(Primitive::Type::Cube);
  entity->transform->SetWorldPos(Math::Vector3{1.5f, 0.5f, 0});
  // Create cylinder mesh entity, no collider
  entity = Primitive::Create(Primitive::Type::Cylinder);
  entity->transform->SetWorldPos(Math::Vector3{-1.5f, 1.f, 0});
  // Create grid mesh entity, with box collider
  //  Grid is horizontal square 5 unit x 0 unit x 5 unit
  entity = Primitive::Create(Primitive::Type::Grid, true);
  // Create quad mesh entity, no collider
  //  Quad is vertical square 1 unit x 1 unit x 0 unit
  entity = Primitive::Create(Primitive::Type::Quad);
  entity->transform->SetWorldPos(Math::Vector3{1.5f, 0.5f, 1});
  entity->transform->SetWorldRot(Math::Vector3{0, 180.f, 0});
  // Create sphere mesh entity, with sphere collider
  entity = Primitive::Create(Primitive::Type::Sphere, true);
  entity->transform->SetWorldPos(Math::Vector3{1.5f, 2.f, 0});
}
}  // namespace Isetta