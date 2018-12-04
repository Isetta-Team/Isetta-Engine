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
Entity* Primitive::Create(Type type, bool withCollider) {
  switch (type) {
    case Type::Capsule:
      return Create(type, "Capsule", withCollider);
    case Type::Cube:
      return Create(type, "Cube", withCollider);
    case Type::Cylinder:
      return Create(type, "Cylinder", withCollider);
    case Type::Grid:
      return Create(type, "Grid", withCollider);
    case Type::Quad:
      return Create(type, "Quad", withCollider);
    case Type::Sphere:
      return Create(type, "Sphere", withCollider);
  };
  return nullptr;
}

Entity* Primitive::Create(Type type, const std::string_view name,
                          bool withCollider) {
  Entity* entity = Entity::Instantiate("");
  switch (type) {
    case Type::Capsule:
      entity->SetName(name);
      entity->AddComponent<MeshComponent>(
          "primitives\\Capsule\\Capsule.scene.xml");
      if (withCollider) entity->AddComponent<CapsuleCollider>();
      break;
    case Type::Cube:
      entity->SetName(name);
      entity->AddComponent<MeshComponent>("primitives\\Cube\\Cube.scene.xml");
      if (withCollider) entity->AddComponent<BoxCollider>();
      break;
    case Type::Cylinder:
      entity->SetName(name);
      entity->AddComponent<MeshComponent>(
          "primitives\\Cylinder\\Cylinder.scene.xml");
      if (withCollider) entity->AddComponent<CapsuleCollider>();
      break;
    case Type::Grid:
      entity->SetName(name);
      entity->AddComponent<MeshComponent>("primitives\\Grid\\Grid.scene.xml");
      if (withCollider)
        entity->AddComponent<BoxCollider>(Math::Vector3::zero,
                                          Math::Vector3{10, 0.1f, 10});
      break;
    case Type::Quad:
      entity->SetName(name);
      entity->AddComponent<MeshComponent>("primitives\\Quad\\Quad.scene.xml");
      if (withCollider)
        entity->AddComponent<BoxCollider>(Math::Vector3::zero,
                                          Math::Vector3{1, 1, 0.1f});
      break;
    case Type::Sphere:
      entity->SetName(name);
      entity->AddComponent<MeshComponent>(
          "primitives\\Sphere\\Sphere.scene.xml");
      if (withCollider) entity->AddComponent<SphereCollider>();
      break;
  };
  return entity;
}
}  // namespace Isetta
