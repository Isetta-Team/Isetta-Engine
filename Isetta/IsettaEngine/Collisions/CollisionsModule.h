/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Util.h"

namespace Isetta::Math {
class Vector3;
}

namespace Isetta {
class CollisionsModule {
 public:
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

  bool Raycast(const class Ray &ray, class RaycastHit *const hitInfo,
               float maxDistance = 0);

  static float ClosestPtRaySegment(const class Ray &, const Math::Vector3 &,
                                   const Math::Vector3 &, float *const,
                                   float *const, Math::Vector3 *const,
                                   Math::Vector3 *const);

  // TODO(Jacob) collision layers
 private:
  CollisionsModule() = default;
  ~CollisionsModule() = default;

  // BVTree tree;
  // TODO(Jacob) remove
  std::vector<class Collider *> colliders;

  std::unordered_set<std::pair<int, int>> collisionPairs;
  // TODO(Jacob) only for color as of now
  std::unordered_map<int, int> collisions{};

  void StartUp();
  void Update(float deltaTime);
  void ShutDown();

  friend class EngineLoop;
  friend class Collider;
  friend class Collisions;

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
  // static float ClosestPtRaySegment(const Ray &, const Math::Vector3 &,
  //                                 const Math::Vector3 &, float *const,
  //                                 float *const, Math::Vector3 *const,
  //                                 Math::Vector3 *const);
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
  ///
  static Math::Vector3 ClosestPtLineOBB(const class Line &,
                                        const class BoxCollider &, float *,
                                        float *);
  static Math::Vector3 Face(int, const class Line &, const class BoxCollider &,
                            const Math::Vector3 &, float *, float *);
  static bool CapsuleAABBIntersect(const Math::Vector3 &start,
                                   const Math::Vector3 &end, const float radius,
                                   const Math::Vector3 &extents, float *t);
  static bool RaySphereIntersectLimited(const Math::Vector3 &start,
                                        const Math::Vector3 &end,
                                        const Math::Vector3 &center,
                                        float radius,
                                        const Math::Vector3 &extents,
                                        const Math::Vector3 &face, float *_t);
  static float SqDistanceToAABB(const Math::Vector3 &min,
                                const Math::Vector3 &max,
                                const Math::Vector3 &center);
  ///
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
