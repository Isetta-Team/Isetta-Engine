/*
 * Copyright (c) 2018 Isetta
 */
#include "Primitive.h"

#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/SphereCollider.h"
#include "Graphics/MeshComponent.h"
#include "Scene/Entity.h"

namespace Isetta {
Entity* Primitive::Create(Type type) {
  Entity* entity = CREATE_ENTITY("");
  switch (type) {
    case Type::Capsule:
      entity->SetName("Capsule");
      entity->AddComponent<MeshComponent>("primitives\\Capsule.scene.xml");
      entity->AddComponent<CapsuleCollider>();
      break;
    case Type::Cube:
      entity->SetName("Cube");
      entity->AddComponent<MeshComponent>("primitives\\Cube.scene.xml");
      entity->AddComponent<BoxCollider>();
      break;
    case Type::Cylinder:
      entity->SetName("Cylinder");
      entity->AddComponent<MeshComponent>("primitives\\Cylinder.scene.xml");
      entity->AddComponent<CapsuleCollider>();
      break;
    case Type::Grid:
      entity->SetName("Grid");
      entity->AddComponent<MeshComponent>("primitives\\Grid.scene.xml");
      break;
    case Type::Quad:
      entity->SetName("Quad");
      entity->AddComponent<MeshComponent>("primitives\\Quad.scene.xml");
      break;
    case Type::Sphere:
      entity->SetName("Sphere");
      entity->AddComponent<MeshComponent>("primitives\\Sphere.scene.xml");
      entity->AddComponent<SphereCollider>();
      break;
  };
  return entity;
}
}  // namespace Isetta
