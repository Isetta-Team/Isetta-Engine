/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_map>
#include <unordered_set>
#include "Collisions/BVTree.h"
#include "Scene/Layers.h"

namespace Isetta::Math {
class Vector3;
}

namespace Isetta {
class CollisionsModule {
 public:
  struct CollisionConfig {
    CVar<float> fatFactor{"collision_fat_factor", 0.2f};
    CVar<Size> bvTreeNodeSize{"bv_tree_node_size", 200};
  };

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
  Array<RaycastHit> RaycastAll(const class Ray &ray, float maxDistance = 0);

  static float ClosestPtRaySegment(const class Ray &, const Math::Vector3 &,
                                   const Math::Vector3 &, float *const,
                                   float *const, Math::Vector3 *const,
                                   Math::Vector3 *const);

  // TODO(Jacob) collision layers
 private:
  CollisionsModule() = default;
  ~CollisionsModule() = default;

  // probably mark them as "still colliding"
  CollisionUtil::ColliderPairSet collidingPairs, lastFramePairs;
  CollisionUtil::ColliderPairSet ignoreColliderPairs;

  BVTree bvTree;
  std::bitset<static_cast<int>(0.5f * Layers::LAYERS_CAPACITY *
                               (Layers::LAYERS_CAPACITY + 1))>
      ignoreCollisionLayer = 0;

  void StartUp();
  void Update(float deltaTime);
  void LateUpdate(float deltaTime);
  void ShutDown();
  Array<Collider *> GetPossibleColliders(Collider *collider) const;

  // Utilities
  bool GetIgnoreLayerCollision(int layer1, int layer2) const;
  void SetIgnoreLayerCollision(int layer1, int layer2, bool ignoreLayer = true);

  static bool Intersection(const Math::Vector3 &, const Math::Vector3 &,
                           const class AABB &);
  static float SqDistPointSegment(const Math::Vector3 &, const Math::Vector3 &,
                                  const Math::Vector3 &);
  static float SqDistPointOBB(const Math::Vector3 &, const class BoxCollider &);
  static float SqDistSegmentOBB(const Math::Vector3 &, const Math::Vector3 &,
                                const BoxCollider &);
  static Math::Vector3 ClosestPtPointSegment(const Math::Vector3 &point,
                                             const Math::Vector3 &p0,
                                             const Math::Vector3 &p1, float *);
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
  static Math::Vector3 ClosestPtPointAABB(const Math::Vector3 &,
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

  friend class EngineLoop;
  friend class Collider;
  friend class Collisions;
  friend class CollisionSolverModule;
  friend class StackAllocator;
};
}  // namespace Isetta
