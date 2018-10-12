/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/DataStructures/AVLTree.h"
#include "Physics/AABB.h"

namespace Isetta {
class PhysicsModule {
 public:
 private:
  PhysicsModule() = default;
  ~PhysicsModule() = default;

  BVTree tree;

  void StartUp();
  void Update(float deltaTime);
  void ShutDown();

  friend class EngineLoop;
  friend class Collider;

  bool Intersection(const class BoxCollider&, const class BoxCollider&);
  bool Intersection(const class BoxCollider&, const class SphereCollider&);
  bool Intersection(const class BoxCollider&, const class CapsuleCollider&);
  bool Intersection(const class SphereCollider&, const class SphereCollider&);
  bool Intersection(const class SphereCollider&, const class BoxCollider&);
  bool Intersection(const class SphereCollider&, const class CapsuleCollider&);
  bool Intersection(const class CapsuleCollider&, const class CapsuleCollider&);
  bool Intersection(const class CapsuleCollider&, const class BoxCollider&);
  bool Intersection(const class CapsuleCollider&, const class SphereCollider&);

  // Utilities
  float SqDistPointSegment(const Math::Vector3&, const Math::Vector3&,
                           const Math::Vector3&);
  float ClosestPointSegmentSegment(const Math::Vector3&, const Math::Vector3&,
                                   const Math::Vector3&, const Math::Vector3&,
                                   float&, float&, Math::Vector3&,
                                   Math::Vector3&);
  Math::Vector3 ClosestPtPointOBB(const Math::Vector3&,
                                  const class BoxCollider&);
};
}  // namespace Isetta
