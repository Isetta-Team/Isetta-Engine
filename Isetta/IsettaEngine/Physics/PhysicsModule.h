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
  static bool Intersection(const Math::Vector3 &, const Math::Vector3 &,
                           const class AABB &);
  static float SqDistPointSegment(const Math::Vector3 &, const Math::Vector3 &,
                                  const Math::Vector3 &);
  static float SqDistPointOBB(const Math::Vector3 &, const class BoxCollider &);
  static float SqDistSegmentOBB(const Math::Vector3 &, const Math::Vector3 &,
                                const BoxCollider &);
  static Math::Vector3 ClosestPtPointSegment(const Math::Vector3 &point,
                                             const Math::Vector3 &p0,
                                             const Math::Vector3 &p1,
                                             float *const t);
  static float ClosestPtSegmentSegment(const Math::Vector3 &,
                                       const Math::Vector3 &,
                                       const Math::Vector3 &,
                                       const Math::Vector3 &, float *const,
                                       float *const, Math::Vector3 *const,
                                       Math::Vector3 *const);
  static Math::Vector3 ClossetPtPointAABB(const Math::Vector3 &,
                                          const class AABB &);
  static Math::Vector3 ClosestPtPointOBB(const Math::Vector3 &,
                                         const class BoxCollider &);
  static Math::Vector3 ClosestPtRayOBB(const class Ray &,
                                       const class BoxCollider &, float *,
                                       float *);
  static Math::Vector3 Face(int, const class Ray &, const class BoxCollider &,
                            const Math::Vector3 &, float *, float *);
  static Math::Vector3 ClosestPtSegmentOBB(const Math::Vector3 &,
                                           const Math::Vector3 &,
                                           const class BoxCollider &);
  inline std::pair<int, int> CollisionPair(int i, int j) {
    if (i < j) {
      return std::make_pair(i, j);
    } else {
      return std::make_pair(j, i);
    }
  }
};
}  // namespace Isetta
