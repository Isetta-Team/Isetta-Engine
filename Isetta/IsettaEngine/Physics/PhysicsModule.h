/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
//#include "Physics/AABB.h"
#include "Util.h"

namespace Isetta::Math {
class Vector3;
}

namespace Isetta {
class PhysicsModule {
 public:
  // TODO(Jacob) can these just be static?
  static bool Intersection(const class BoxCollider &,
                           const class BoxCollider &);
  static bool Intersection(const class BoxCollider &,
                           const class SphereCollider &);
  static bool Intersection(const class BoxCollider &,
                           const class CapsuleCollider &);
  static bool Intersection(const class SphereCollider &,
                           const class SphereCollider &);
  static bool Intersection(const class SphereCollider &,
                           const class BoxCollider &);
  static bool Intersection(const class SphereCollider &,
                           const class CapsuleCollider &);
  static bool Intersection(const class CapsuleCollider &,
                           const class CapsuleCollider &);
  static bool Intersection(const class CapsuleCollider &,
                           const class BoxCollider &);
  static bool Intersection(const class CapsuleCollider &,
                           const class SphereCollider &);

 private:
  PhysicsModule() = default;
  ~PhysicsModule() = default;

  // BVTree tree;
  // TODO(Jacob) remove
  std::vector<class Collider *> colliders;

  std::unordered_set<std::pair<int, int>> collisionPairs;
  // TODO(Jacob) only for color as of now
  std::unordered_map<int, int> collisions{};

  void StartUp();
  void Update(float deltaTime);
  void ShutDown();

  void OnCollisionEnter(class Collider *const);
  void OnCollisionStay(class Collider *const);
  void OnCollisionExit(class Collider *const);

  friend class EngineLoop;
  friend class Collider;

  // Utilities
  float SqDistPointSegment(const Math::Vector3 &, const Math::Vector3 &,
                           const Math::Vector3 &);
  float ClosestPointSegmentSegment(const Math::Vector3 &, const Math::Vector3 &,
                                   const Math::Vector3 &, const Math::Vector3 &,
                                   float *const, float *const,
                                   Math::Vector3 *const, Math::Vector3 *const);
  Math::Vector3 ClosestPtPointOBB(const Math::Vector3 &,
                                  const class BoxCollider &);
};
}  // namespace Isetta
