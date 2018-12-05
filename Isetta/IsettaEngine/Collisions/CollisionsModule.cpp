/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/CollisionsModule.h"

#include "Collisions/AABB.h"
#include "Collisions/RaycastHit.h"
#include "Core/Geometry/Ray.h"
#include "Core/Math/Matrix3.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "brofiler/ProfilerCore/Brofiler.h"

#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/Collider.h"
#include "Collisions/CollisionHandler.h"
#include "Collisions/Collisions.h"
#include "Collisions/SphereCollider.h"

#include "Collisions/CollisionSolverModule.h"
#include "Core/Config/Config.h"

namespace Isetta {
void CollisionsModule::StartUp() {
  Collider::collisionsModule = this;
  Collider::fatFactor = CONFIG_VAL(collisionConfig.fatFactor);
  Collisions::collisionsModule = this;
  CollisionSolverModule::collisionsModule = this;
}

void CollisionsModule::Update(float deltaTime) {
  BROFILER_CATEGORY("Collision Update", Profiler::Color::Orchid);

  bvTree.Update();

  // By the end of the checking loop, pairs left in the lastFramePairs
  // are those who are no longer colliding
  lastFramePairs = collidingPairs;

  collidingPairs.clear();

  auto pairs = bvTree.GetCollisionPairs();
  for (const auto &pair : pairs) {
    Collider *collider1 = pair.first;
    Collider *collider2 = pair.second;
    // Ignore Single/Layer Collisions continue
    if (collider1->entity == collider2->entity ||
        ignoreColliderPairs.find(pair) != ignoreColliderPairs.end() ||
        GetIgnoreLayerCollision(collider1->entity->GetLayerIndex(),
                                collider2->entity->GetLayerIndex()))
      continue;

    CollisionHandler *handler1 = collider1->GetHandler();
    CollisionHandler *handler2 = collider2->GetHandler();

    // Trigger colliders with no handler shouldn't even be intersection tested
    if ((collider1->isTrigger || collider2->isTrigger) && handler1 &&
        handler2 && handler1 == handler2)
      continue;

    if (collider1->Intersection(collider2)) collidingPairs.insert(pair);
  }
}

void CollisionsModule::LateUpdate(float deltaTime) {
  for (const auto &pair : collidingPairs) {
    Collider *collider1 = pair.first;
    Collider *collider2 = pair.second;

    CollisionHandler *handler1 = collider1->GetHandler();
    CollisionHandler *handler2 = collider2->GetHandler();
    if (handler1 == handler2) continue;

    auto it = lastFramePairs.find(pair);

    if (it != lastFramePairs.end()) {
      // pair was colliding last frame
      if (handler1) handler1->OnCollisionStay(collider2);
      if (handler2) handler2->OnCollisionStay(collider1);
      lastFramePairs.erase(it);
    } else {
      // pair is new
      if (handler1) handler1->OnCollisionEnter(collider2);
      if (handler2) handler2->OnCollisionEnter(collider1);
    }
  }

  for (const auto &pair : lastFramePairs) {
    Collider *collider1 = pair.first;
    Collider *collider2 = pair.second;
    if (!collider1->entity || !collider2->entity) continue;

    CollisionHandler *handler1 = collider1->GetHandler();
    CollisionHandler *handler2 = collider2->GetHandler();

    if (handler1) handler1->OnCollisionExit(collider2);
    if (handler2) handler2->OnCollisionExit(collider1);
  }
}

void CollisionsModule::ShutDown() {}

Array<Collider *> CollisionsModule::GetPossibleColliders(
    Collider *collider) const {
  return bvTree.GetPossibleColliders(collider);
}

bool CollisionsModule::Intersection(const BoxCollider &a,
                                    const BoxCollider &b) {
  PROFILE
  float ra, rb;
  Math::Matrix3 rot, absRot;

  for (int i = 0; i < Math::Matrix3::ROW_COUNT; ++i) {
    for (int j = 0; j < Math::Matrix3::ROW_COUNT; ++j) {
      rot[i][j] =
          Math::Vector3::Dot(a.transform->GetAxis(i), b.transform->GetAxis(j));
    }
  }
  Math::Vector3 t = b.GetWorldCenter() - a.GetWorldCenter();
  t = Math::Vector3{Math::Vector3::Dot(t, a.transform->GetLeft()),
                    Math::Vector3::Dot(t, a.transform->GetUp()),
                    Math::Vector3::Dot(t, a.transform->GetForward())};
  for (int i = 0; i < Math::Matrix3::ROW_COUNT; ++i) {
    for (int j = 0; j < Math::Matrix3::ROW_COUNT; ++j) {
      absRot[i][j] = Math::Util::Abs(rot[i][j]) + Math::Util::EPSILON;
    }
  }

  Math::Vector3 aExtents = a.GetWorldExtents();
  Math::Vector3 bExtents = b.GetWorldExtents();
  // Test axes L = A0, L = A1, L = A2
  for (int i = 0; i < 3; ++i) {
    ra = aExtents[i];
    rb = bExtents[0] * absRot[i][0] + bExtents[1] * absRot[i][1] +
         bExtents[2] * absRot[i][2];
    if (Math::Util::Abs(t[i]) > ra + rb) return 0;
  }
  // Test axes L = B0, L = B1, L = B2
  for (int i = 0; i < 3; ++i) {
    ra = aExtents[0] * absRot[0][i] + aExtents[1] * absRot[1][i] +
         aExtents[2] * absRot[2][i];
    rb = bExtents[i];
    if (Math::Util::Abs(t[0] * rot[0][i] + t[1] * rot[1][i] +
                        t[2] * rot[2][i]) > ra + rb)
      return 0;
  }
  // Test axis L = A0 x B0
  ra = aExtents[1] * absRot[2][0] + aExtents[2] * absRot[1][0];
  rb = bExtents[1] * absRot[0][2] + bExtents[2] * absRot[0][1];
  if (Math::Util::Abs(t[2] * rot[1][0] - t[1] * rot[2][0]) > ra + rb) {
    return false;
  }
  // Test axis L = A0 x B1
  ra = aExtents[1] * absRot[2][1] + aExtents[2] * absRot[1][1];
  rb = bExtents[0] * absRot[0][2] + bExtents[2] * absRot[0][0];
  if (Math::Util::Abs(t[2] * rot[1][1] - t[1] * rot[2][1]) > ra + rb) {
    return false;
  }
  // Test axis L = A0 x B2
  ra = aExtents[1] * absRot[2][2] + aExtents[2] * absRot[1][2];
  rb = bExtents[0] * absRot[0][1] + bExtents[1] * absRot[0][0];
  if (Math::Util::Abs(t[2] * rot[1][2] - t[1] * rot[2][2]) > ra + rb) {
    return false;
  }
  // Test axis L = A1 x B0
  ra = aExtents[0] * absRot[2][0] + aExtents[2] * absRot[0][0];
  rb = bExtents[1] * absRot[1][2] + bExtents[2] * absRot[1][1];

  if (Math::Util::Abs(t[0] * rot[2][0] - t[2] * rot[0][0]) > ra + rb) {
    return false;
  }
  // Test axis L = A1 x B1
  ra = aExtents[0] * absRot[2][1] + aExtents[2] * absRot[0][1];
  rb = bExtents[0] * absRot[1][2] + bExtents[2] * absRot[1][0];
  if (Math::Util::Abs(t[0] * rot[2][1] - t[2] * rot[0][1]) > ra + rb) {
    return false;
  }
  // Test axis L = A1 x B2
  ra = aExtents[0] * absRot[2][2] + aExtents[2] * absRot[0][2];
  rb = bExtents[0] * absRot[1][1] + bExtents[1] * absRot[1][0];
  if (Math::Util::Abs(t[0] * rot[2][2] - t[2] * rot[0][2]) > ra + rb) {
    return false;
  }
  // Test axis L = A2 x B0
  ra = aExtents[0] * absRot[1][0] + aExtents[1] * absRot[0][0];
  rb = bExtents[1] * absRot[2][2] + bExtents[2] * absRot[2][1];
  if (Math::Util::Abs(t[1] * rot[0][0] - t[0] * rot[1][0]) > ra + rb) {
    return false;
  }
  // Test axis L = A2 x B1
  ra = aExtents[0] * absRot[1][1] + aExtents[1] * absRot[0][1];
  rb = bExtents[0] * absRot[2][2] + bExtents[2] * absRot[2][0];
  if (Math::Util::Abs(t[1] * rot[0][1] - t[0] * rot[1][1]) > ra + rb) {
    return false;
  }
  // Test axis L = A2 x B2
  ra = aExtents[0] * absRot[1][2] + aExtents[1] * absRot[0][2];
  rb = bExtents[0] * absRot[2][1] + bExtents[1] * absRot[2][0];
  if (Math::Util::Abs(t[1] * rot[0][2] - t[0] * rot[1][2]) > ra + rb) {
    return false;
  }
  // Since no separating axis is found, the OBBs must be intersecting
  return true;
}

bool CollisionsModule::Intersection(const BoxCollider &box,
                                    const SphereCollider &sphere) {
  PROFILE
  Math::Vector3 center = sphere.GetWorldCenter();
  Math::Vector3 pt = ClosestPtPointOBB(center, box);
  Math::Vector3 to = pt - center;
  return Math::Vector3::Dot(to, to) <=
         sphere.GetWorldRadius() * sphere.GetWorldRadius();
}
bool CollisionsModule::Intersection(const BoxCollider &box,
                                    const CapsuleCollider &capsule) {
  PROFILE
  Math::Matrix4 rot, scale;
  float radiusScale = capsule.GetWorldCapsule(&rot, &scale);
  Math::Vector3 dir =
      (Math::Vector3)(rot * scale *
                      Math::Matrix4::Scale(
                          Math::Vector3{capsule.height - 2 * capsule.radius}) *
                      Math::Vector4{0, 1, 0, 0}) *
      .5;
  Math::Vector3 p0 = capsule.GetWorldCenter() - dir;
  Math::Vector3 p1 = capsule.GetWorldCenter() + dir;
  // DebugDraw::Point(p0, Color::magenta, 10, 0.1f);
  // DebugDraw::Point(p1, Color::magenta, 10, 0.1f);
  return SqDistSegmentOBB(p0, p1, box) <=
         Math::Util::Square(capsule.radius * radiusScale);

  // https://github.com/vancegroup-mirrors/open-dynamics-engine-svnmirror/blob/master/ode/src/capsule.cpp

  // Not working
  // Math::Vector3 localP0 = box.transform->LocalPosFromWorldPos(p0);
  // Math::Vector3 localP1 = box.transform->LocalPosFromWorldPos(p1);

  // float t;
  // bool intersect =
  //    CapsuleAABBIntersect(localP0, localP1, capsule.radius * radiusScale,
  //                         box.GetWorldExtents(), &t);
  // t = Math::Util::Clamp01(t);
  // DebugDraw::Point(p0 + t * (p1 - p0), Color::magenta, 10, 0.1, false);
  // return intersect;
}
bool CollisionsModule::Intersection(const SphereCollider &a,
                                    const SphereCollider &b) {
  PROFILE
  return (a.GetWorldCenter() - b.GetWorldCenter()).SqrMagnitude() <=
         Math::Util::Square(a.GetWorldRadius() + b.GetWorldRadius());
}
bool CollisionsModule::Intersection(const SphereCollider &sphere,
                                    const BoxCollider &box) {
  PROFILE
  return Intersection(box, sphere);
}
bool CollisionsModule::Intersection(const SphereCollider &sphere,
                                    const CapsuleCollider &capsule) {
  PROFILE
  Math::Matrix4 rot, scale;
  float radiusScale = capsule.GetWorldCapsule(&rot, &scale);
  Math::Vector3 dir =
      (Math::Vector3)(rot * scale *
                      Math::Matrix4::Scale(
                          Math::Vector3{capsule.height - 2 * capsule.radius}) *
                      Math::Vector4{0, 1, 0, 0}) *
      .5;
  Math::Vector3 p0 = capsule.GetWorldCenter() - dir;
  Math::Vector3 p1 = capsule.GetWorldCenter() + dir;
  float distSq = SqDistPointSegment(p0, p1, sphere.GetWorldCenter());
  return distSq <= Math::Util::Square(sphere.GetWorldRadius() +
                                      capsule.radius * radiusScale);
}
bool CollisionsModule::Intersection(const CapsuleCollider &a,
                                    const CapsuleCollider &b) {
  PROFILE
  Math::Matrix4 aRot, aScale, bRot, bScale;
  float arScale = a.GetWorldCapsule(&aRot, &aScale);
  float brScale = b.GetWorldCapsule(&bRot, &bScale);
  Math::Vector3 aDir =
      (Math::Vector3)(
          aRot * aScale *
          Math::Matrix4::Scale(Math::Vector3{a.height - 2 * a.radius}) *
          Math::Vector4{0, 1, 0, 0}) *
      .5;
  Math::Vector3 bDir =
      (Math::Vector3)(
          bRot * bScale *
          Math::Matrix4::Scale(Math::Vector3{b.height - 2 * b.radius}) *
          Math::Vector4{0, 1, 0, 0}) *
      .5;
  Math::Vector3 aP0 = a.GetWorldCenter() - aDir;
  Math::Vector3 aP1 = a.GetWorldCenter() + aDir;
  Math::Vector3 bP0 = b.GetWorldCenter() - bDir;
  Math::Vector3 bP1 = b.GetWorldCenter() + bDir;

  float s, t;
  Math::Vector3 ac, bc;
  float distSq = ClosestPtSegmentSegment(aP0, aP1, bP0, bP1, &s, &t, &ac, &bc);

  return distSq <= Math::Util::Square(a.radius * arScale + b.radius * brScale);
}
bool CollisionsModule::Intersection(const CapsuleCollider &capsule,
                                    const BoxCollider &box) {
  PROFILE
  return Intersection(box, capsule);
}
bool CollisionsModule::Intersection(const CapsuleCollider &capsule,
                                    const SphereCollider &sphere) {
  PROFILE
  return Intersection(sphere, capsule);
}
bool CollisionsModule::Raycast(const Ray &ray, RaycastHit *const hitInfo,
                               float maxDistance) {
  PROFILE
  return bvTree.Raycast(ray, hitInfo, maxDistance);
}
Array<RaycastHit> CollisionsModule::RaycastAll(const Ray &ray,
                                               float maxDistance) {
  PROFILE
  return bvTree.RaycastAll(ray, maxDistance);
}
bool CollisionsModule::GetIgnoreLayerCollision(int layer1, int layer2) const {
  if (layer1 < layer2)
    return ignoreCollisionLayer.test(layer1 * Layers::LAYERS_CAPACITY + layer2);
  else
    return ignoreCollisionLayer.test(layer2 * Layers::LAYERS_CAPACITY + layer1);
}
void CollisionsModule::SetIgnoreLayerCollision(int layer1, int layer2,
                                               bool ignoreLayer) {
  if (layer1 < layer2)
    ignoreCollisionLayer.set(layer1 * Layers::LAYERS_CAPACITY + layer2,
                             ignoreLayer);
  else
    ignoreCollisionLayer.set(layer2 * Layers::LAYERS_CAPACITY + layer1,
                             ignoreLayer);
}
bool CollisionsModule::Intersection(const Math::Vector3 &p0,
                                    const Math::Vector3 &p1, const AABB &aabb) {
  Math::Vector3 c = aabb.GetSize();
  Math::Vector3 e = aabb.GetMax() - c;
  Math::Vector3 m = 0.5f * (p0 + p1);
  Math::Vector3 d = p1 - m;
  m -= c;
  float adx = Math::Util::Abs(d.x);
  if (Math::Util::Abs(m.x) > e.x + adx) {
    return false;
  }
  float ady = Math::Util::Abs(d.y);
  if (Math::Util::Abs(m.y) > e.y + ady) {
    return false;
  }
  float adz = Math::Util::Abs(d.z);
  if (Math::Util::Abs(m.z) > e.z + adz) {
    return false;
  }

  adx += Math::Util::EPSILON;
  ady += Math::Util::EPSILON;
  adz += Math::Util::EPSILON;

  if (Math::Util::Abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) {
    return false;
  }
  if (Math::Util::Abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) {
    return false;
  }
  if (Math::Util::Abs(m.y * d.z - m.y * d.x) > e.x * ady + e.y * adx) {
    return false;
  }

  return true;
}
float CollisionsModule::SqDistPointSegment(const Math::Vector3 &a,
                                           const Math::Vector3 &b,
                                           const Math::Vector3 &c) {
  Math::Vector3 ab = b - a, ac = c - a, bc = c - b;
  float e = Math::Vector3::Dot(ac, ab);
  if (e <= 0) return Math::Vector3::Dot(ac, ac);
  float f = Math::Vector3::Dot(ab, ab);
  if (e >= f) return Math::Vector3::Dot(bc, bc);
  return Math::Vector3::Dot(ac, ac) - e * e / f;
}
float CollisionsModule::SqDistPointOBB(const Math::Vector3 &point,
                                       const BoxCollider &box) {
  Math::Vector3 closest = ClosestPtPointOBB(point, box);
  // DebugDraw::Point(closest, Color::blue, 20, 0.1, false);
  float sqDist = Math::Vector3::Dot(closest - point, closest - point);
  return sqDist;
}
float CollisionsModule::SqDistSegmentOBB(const Math::Vector3 &p0,
                                         const Math::Vector3 &p1,
                                         const BoxCollider &box) {
  Line line = Line{p0, p1 - p0};
  // Line line = Line{p0, p1 - p0};
  float t, distSq;
  Math::Vector3 pt = ClosestPtLineOBB(line, box, &t, &distSq);
  // DebugDraw::Point(pt, Color::red, 10, 0.1);
  // LOG_INFO(Debug::Channel::Collisions, "(%f)", t);
  // LOG_INFO(Debug::Channel::Collisions, "(%f, %f, %f)", pt.x, pt.y, pt.z);
  if (t <= 0) {
    distSq = SqDistPointOBB(p0, box);
    // DebugDraw::Point(p0, Color::white, 10, 0.1, false);
  } else if (t >= 1) {
    distSq = SqDistPointOBB(p1, box);
    // DebugDraw::Point(p1, Color::white, 10, 0.1, false);
  } else {
    // DebugDraw::Point(line.GetPoint(t), Color::white, 10, 0.1, false);
  }
  return distSq;
}
Math::Vector3 CollisionsModule::ClosestPtPointSegment(
    const Math::Vector3 &point, const Math::Vector3 &p0,
    const Math::Vector3 &p1, float *_t) {
  float &t = *_t;
  Math::Vector3 to = p1 - p0;
  t = Math::Vector3::Dot(point - p0, to) / Math::Vector3::Dot(to, to);
  if (t < 0.0f) {
    t = 0.0f;
  }
  if (t > 1.0f) {
    t = 1.0f;
  }
  return p0 + t * to;
}
float CollisionsModule::ClosestPtSegmentSegment(
    const Math::Vector3 &p0, const Math::Vector3 &p1, const Math::Vector3 &q0,
    const Math::Vector3 &q1, float *const tP, float *const tQ,
    Math::Vector3 *const cP, Math::Vector3 *const cQ) {
  float &s = *tP, &t = *tQ;
  Math::Vector3 &c1 = *cP, &c2 = *cQ;

  Math::Vector3 d1 = p1 - p0;  // Direction vector of segment S1
  Math::Vector3 d2 = q1 - q0;  // Direction vector of segment S2
  Math::Vector3 r = p0 - q0;
  float a = Math::Vector3::Dot(
      d1, d1);  // Squared length of segment S1, always nonnegative
  float e = Math::Vector3::Dot(
      d2, d2);  // Squared length of segment S2, always nonnegative
  float f = Math::Vector3::Dot(d2, r);

  // Check if either or both segments degenerate into points
  if (a <= Math::Util::EPSILON && e <= Math::Util::EPSILON) {
    // Both segments degenerate into points
    s = t = 0.0f;
    c1 = p0;
    c2 = q0;
    return Math::Vector3::Dot(c1 - c2, c1 - c2);
  }
  if (a <= Math::Util::EPSILON) {
    // First segment degenerates into a point
    s = 0.0f;
    t = f / e;  // s = 0 => t = (b*s + f) / e = f / e
    t = Math::Util::Clamp01(t);
  } else {
    float c = Math::Vector3::Dot(d1, r);
    if (e <= Math::Util::EPSILON) {
      // Second segment degenerates into a point
      t = 0.0f;
      s = Math::Util::Clamp01(-c / a);  // t = 0 => s = (b*t - c) / a = -c / a
    } else {
      // The general nondegenerate case starts here
      float b = Math::Vector3::Dot(d1, d2);
      float denom = a * e - b * b;  // Always nonnegative
                                    // If segments not parallel, compute closest
                                    // point on L1 to L2 and clamp to segment
                                    // S1. Else pick arbitrary s (here 0)
      if (denom != 0.0f) {
        s = Math::Util::Clamp01((b * f - c * e) / denom);
      } else {
        s = 0.0f;
      }
      // Compute point on L2 closest to S1(s) using
      // t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
      t = (b * s + f) / e;
      // If t in [0,1] done. Else clamp t, recompute s for the new value
      // of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
      // and clamp s to [0, 1]
      if (t < 0.0f) {
        t = 0.0f;
        s = Math::Util::Clamp01(-c / a);
      } else if (t > 1.0f) {
        t = 1.0f;
        s = Math::Util::Clamp01((b - c) / a);
      }
    }
  }
  c1 = p0 + d1 * s;
  c2 = q0 + d2 * t;
  return Math::Vector3::Dot(c1 - c2, c1 - c2);
}
float CollisionsModule::ClosestPtRaySegment(
    const Ray &ray, const Math::Vector3 &p0, const Math::Vector3 &p1,
    float *const _tRay, float *const _tSeg, Math::Vector3 *const _cRay,
    Math::Vector3 *const _cSeg) {
  float &tRay = *_tRay, &tSeg = *_tSeg;
  Math::Vector3 &cRay = *_cRay, &cSeg = *_cSeg;

  Math::Vector3 d = p1 - p0;  // Direction vector of segment S1
  Math::Vector3 r = p0 - ray.GetOrigin();
  float a = Math::Vector3::Dot(
      d, d);  // Squared length of segment S1, always nonnegative
  float e =
      Math::Vector3::Dot(ray.GetDirection(),
                         ray.GetDirection());  // Squared length of segment
                                               // S2, always nonnegative
  float f = Math::Vector3::Dot(ray.GetDirection(), r);

  if (a <= Math::Util::EPSILON) {
    // First segment degenerates into a point
    tSeg = 0.0f;
    tRay = f / e;  // s = 0 => t = (b*s + f) / e = f / e
    tRay = Math::Util::Max(tRay, 0.f);
  } else {
    float c = Math::Vector3::Dot(d, r);
    // The general nondegenerate case starts here
    float b = Math::Vector3::Dot(d, ray.GetDirection());
    float denom = a * e - b * b;  // Always nonnegative
                                  // If segments not parallel, compute closest
                                  // point on L1 to L2 and clamp to segment
                                  // S1. Else pick arbitrary s (here 0)
    if (denom != 0.0f) {
      tSeg = Math::Util::Clamp01((b * f - c * e) / denom);
    } else {
      tSeg = 0.0f;
    }
    // Compute point on L2 closest to S1(s) using
    // t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
    tRay = (b * tSeg + f) / e;
    // If t in [0,1] done. Else clamp t, recompute s for the new value
    // of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
    // and clamp s to [0, 1]
    if (tRay < 0.0f) {
      tRay = 0.0f;
      tSeg = Math::Util::Clamp01(-c / a);
    }
  }
  cRay = ray.GetPoint(tRay);
  cSeg = p0 + d * tSeg;
  return Math::Vector3::Dot(cRay - cSeg, cRay - cSeg);
}
Math::Vector3 CollisionsModule::ClosestPtPointAABB(const Math::Vector3 &point,
                                                   const AABB &aabb) {
  Math::Vector3 pt;
  pt.x = Math::Util::Min(Math::Util::Max(point.x, aabb.GetMin().x),
                         aabb.GetMax().x);
  pt.y = Math::Util::Min(Math::Util::Max(point.y, aabb.GetMin().y),
                         aabb.GetMax().y);
  pt.z = Math::Util::Min(Math::Util::Max(point.z, aabb.GetMin().z),
                         aabb.GetMax().z);
  return pt;
}
Math::Vector3 CollisionsModule::ClosestPtPointOBB(const Math::Vector3 &point,
                                                  const BoxCollider &box) {
  Math::Vector3 d = point - (box.GetWorldCenter());
  Math::Vector3 pt = box.GetWorldCenter();

  Math::Vector3 extents = box.GetWorldExtents();
  for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
    float dist = Math::Vector3::Dot(d, box.transform->GetAxis(i));
    if (dist > extents[i]) {
      dist = extents[i];
    } else if (dist < -extents[i]) {
      dist = -extents[i];
    }
    pt += dist * box.transform->GetAxis(i);
  }
  return pt;
}
Math::Vector3 CollisionsModule::ClosestPtLineOBB(const Line &line,
                                                 const BoxCollider &box,
                                                 float *_t, float *_distSq) {
  Math::Vector3 o = box.transform->LocalPosFromWorldPos(line.GetOrigin());
  Math::Vector3 dir = box.transform->LocalDirFromWorldDir(line.GetDirection());
  float &t = *_t;
  float &distSq = *_distSq;
  distSq = 0;

  int perp = 0;
  for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
    if (Math::Util::Abs(Math::Vector3::Dot(
            dir, Math::Matrix3::identity.GetRow(i))) < Math::Util::EPSILON)
      ++perp;
  }
  switch (perp) {
    case 3:
      // LOG_INFO(Debug::Channel::Collisions, "Case 3");
      distSq = t = 0;
      return ClosestPtPointOBB(line.GetOrigin(), box);
    case 2: {
      // LOG_INFO(Debug::Channel::Collisions, "Case 2");
      int x = -1;
      for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
        if (Math::Util::Abs(
                Math::Vector3::Dot(dir, Math::Matrix3::identity.GetRow(i))) >
            Math::Util::EPSILON) {
          x = i;
          break;
        }
      }
      int y = (x + 1) % Math::Vector3::ELEMENT_COUNT,
          z = (x + 2) % Math::Vector3::ELEMENT_COUNT;

      Math::Vector3 extents = box.GetWorldExtents();
      Math::Vector3 pt;
      pt[x] = o[x] < 0 ? -extents[x] : extents[x];
      pt[y] = o[y];
      pt[z] = o[z];
      t = (pt[x] - o[x]) / dir[x];
      if (o[y] < -extents[y]) {
        float delta = o[y] + extents[y];
        distSq += delta * delta;
        pt[y] = -extents[y];
      } else if (o[y] > extents[y]) {
        float delta = o[y] - extents[y];
        distSq += delta * delta;
        pt[y] = extents[y];
      }

      if (o[z] < -extents[z]) {
        float delta = o[z] + extents[z];
        distSq += delta * delta;
        pt[z] = -extents[z];
      } else if (o[z] > extents[z]) {
        float delta = o[z] - extents[z];
        distSq += delta * delta;
        pt[z] = extents[z];
      }
      // dist = 0 means intersection
      return box.transform->WorldPosFromLocalPos(pt);
    }
    case 1: {
      // LOG_INFO(Debug::Channel::Collisions, "Case 1");
      int z = -1;
      for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
        if (Math::Util::Abs(
                Math::Vector3::Dot(dir, Math::Matrix3::identity.GetRow(i))) <
            Math::Util::EPSILON) {
          z = i;
          break;
        }
      }
      int x = (z + 1) % Math::Vector3::ELEMENT_COUNT,
          y = (z + 2) % Math::Vector3::ELEMENT_COUNT;

      Math::Vector3 extents = box.GetWorldExtents();
      Math::Vector3 minusExtents = o - extents;
      float prod0 = dir[y] * minusExtents[x];
      float prod1 = dir[x] * minusExtents[y];
      Math::Vector3 pt;
      pt[z] = o[z];

      if (o[z] < -extents[z]) {
        float delta = o[z] + extents[z];
        distSq += delta * delta;
        pt[z] = -extents.z;
      } else if (o[z] > extents[z]) {
        float delta = o[z] - extents[z];
        distSq += delta * delta;
        pt[z] = extents[z];
      }

      Ray r{o, dir};
      Math::Vector3 p0{}, p1{};
      p0[x] = o[x] < 0 ? -extents[x] : extents[x];
      p0[y] = o[y] < 0 ? -extents[y] : extents[y];

      p0[z] = pt[z];
      distSq = INFINITY;
      // TODO(Jacob) could be optimized maybe
      for (int i = 0; i < 2; ++i) {
        p1 = p0;
        float tRay, tSeg;
        Math::Vector3 ptRay, ptSeg;
        p1[(x + i) % 3] = -p0[(x + i) % 3];
        float dist =
            ClosestPtRaySegment(r, p0, p1, &tRay, &tSeg, &ptRay, &ptSeg);
        if (dist < distSq) {
          t = tRay;
          pt = ptSeg;
          distSq = dist;
        }
      }
      t /= line.GetDirection().Magnitude();
      return box.transform->WorldPosFromLocalPos(pt);
    }
    case 0: {  // assuming line is positive
      // LOG_INFO(Debug::Channel::Collisions, "Case 0");
      // TODO(Jacob) does not work in completeness
      // http://lib.ysu.am/open_books/312121.pdf p 450/487 - 465/502
      Math::Vector3 extents = box.GetWorldExtents();
      Math::Vector3 minusExtents = o - extents;
      float dyEx = dir.y * minusExtents.x;
      float dxEy = dir.x * minusExtents.y;

      Math::Vector3 pt = extents;
      if (dyEx >= dxEy) {
        float dzEx = dir.z * minusExtents.x;
        float dxEz = dir.x * minusExtents.z;

        if (dzEx >= dxEz) {  // line intersects x = box.extent.x plane
          pt = Face(0, Line{o, dir}, box, minusExtents, &t, &distSq);
        } else {  // line intersects z = box.extent.z plane
          pt = Face(2, Line{o, dir}, box, minusExtents, &t, &distSq);
        }
      } else {
        float dzEy = dir.z * minusExtents.y;
        float dyEz = dir.y * minusExtents.z;

        if (dzEy >= dyEz) {  // line intersects y = box.extent.y plane
          pt = Face(1, Line{o, dir}, box, minusExtents, &t, &distSq);
        } else {  // line intersects z = box.extent.z plane
          pt = Face(2, Line{o, dir}, box, minusExtents, &t, &distSq);
        }
      }
      return box.transform->WorldPosFromLocalPos(pt);
    }
  };
}
Math::Vector3 CollisionsModule::Face(int x, const Line &line,
                                     const BoxCollider &box,
                                     const Math::Vector3 &minusExtents,
                                     float *_t, float *_distSq) {
  int y = (x + 1) % Math::Vector3::ELEMENT_COUNT,
      z = (x + 2) % Math::Vector3::ELEMENT_COUNT;
  float &t = *_t, &distSq = *_distSq;
  Math::Vector3 pt = line.GetOrigin();
  distSq = 0;

  Math::Vector3 extents = box.GetWorldExtents();
  Math::Vector3 plusExtents = line.GetOrigin() + extents;

  if (line.GetDirection()[x] * plusExtents[y] >=
      line.GetDirection()[y] * minusExtents[x]) {  // region 0, 5, or 4
    if (line.GetDirection()[x] * plusExtents[z] >=
        line.GetDirection()[z] * minusExtents[x]) {  // region 0
      pt[x] = extents[x];
      float inv = 1.0f / line.GetDirection()[x];
      pt[y] -= line.GetDirection()[y] * minusExtents[x] * inv;
      pt[z] -= line.GetDirection()[z] * minusExtents[x] * inv;
      t = -minusExtents[x] * inv;  // todo what?
    } else {                       // region 4 or 5
      float lSqr = line.GetDirection()[x] * line.GetDirection()[x] +
                   line.GetDirection()[z] * line.GetDirection()[z];
      float tmp =
          lSqr * plusExtents[y] -
          line.GetDirection()[y] * (line.GetDirection()[x] * minusExtents[x] +
                                    line.GetDirection()[z] * plusExtents[z]);
      if (tmp <= 2 * lSqr * extents[y]) {  // region 4
        tmp = plusExtents[y] - (tmp / lSqr);
        lSqr += line.GetDirection()[y] * line.GetDirection()[y];
        float delta = line.GetDirection()[x] * minusExtents[x] +
                      line.GetDirection()[y] * tmp +
                      line.GetDirection()[z] * plusExtents[z];
        t = -delta / lSqr;
        distSq += minusExtents[x] * minusExtents[x] + tmp * tmp +
                  plusExtents[z] * plusExtents[z] + delta * t;
        pt[x] = extents[x];
        pt[y] = t - extents[y];
        pt[z] = -extents[z];
      } else {  // region 5
        lSqr += line.GetDirection()[y] * line.GetDirection()[y];
        float delta = line.GetDirection()[x] * minusExtents[x] +
                      line.GetDirection()[y] * minusExtents[y] +
                      line.GetDirection()[z] * plusExtents[z];
        t = -delta / lSqr;
        distSq += minusExtents[x] * minusExtents[x] +
                  minusExtents[y] * minusExtents[y] +
                  plusExtents[z] * plusExtents[z] + delta * t;
        pt[x] = extents[x];
        pt[y] = extents[y];
        pt[z] = -extents[z];
      }
    }
  } else {  // region 1 or 2
    if (line.GetDirection()[x] * plusExtents[z] >=
        line.GetDirection()[z] * minusExtents[x]) {
      float lSqr = line.GetDirection()[x] * line.GetDirection()[x] +
                   line.GetDirection()[y] * line.GetDirection()[y];
      float tmp = lSqr * plusExtents[z] -
                  line.GetDirection()[z] *
                      (line.GetDirection()[x] * minusExtents[x] +
                       line.GetDirection()[y] * plusExtents[y]);  // todo what?
      if (tmp <= 2 * lSqr * extents[z]) {                         // region 2
        tmp = plusExtents[z] - (tmp / lSqr);
        lSqr += line.GetDirection()[z] * line.GetDirection()[z];
        float delta = line.GetDirection()[x] * minusExtents[x] +
                      line.GetDirection()[y] * plusExtents[y] +
                      line.GetDirection()[z] * tmp;
        t = -delta / lSqr;
        distSq += minusExtents[x] * minusExtents[x] +
                  minusExtents[y] * minusExtents[y] + tmp * tmp + delta * t;
        pt[x] = extents[x];
        pt[y] = -extents[y];
        pt[z] = t - extents[z];
      } else {  // region 1
        lSqr += line.GetDirection()[z] * line.GetDirection()[z];
        float delta = line.GetDirection()[x] * minusExtents[x] +
                      line.GetDirection()[y] * minusExtents[y] +
                      line.GetDirection()[z] * plusExtents[z];
        t = -delta / lSqr;
        distSq += minusExtents[x] * minusExtents[x] +
                  plusExtents[y] * plusExtents[y] +
                  minusExtents[z] * minusExtents[z] + delta * t;
        pt[x] = extents[x];
        pt[y] = -extents[y];
        pt[z] = extents[z];
      }
    } else {
      float lSqr = line.GetDirection()[x] * line.GetDirection()[x] +
                   line.GetDirection()[z] * line.GetDirection()[z];
      float tmp =
          lSqr * plusExtents[y] -
          line.GetDirection()[y] * (line.GetDirection()[x] * minusExtents[x] +
                                    line.GetDirection()[z] * plusExtents[z]);
      if (tmp >= 0) {                        // region 4 or 5 -- copied
        if (tmp <= 2 * lSqr * extents[z]) {  // region 4 -- copied
          tmp = plusExtents[y] - (tmp / lSqr);
          lSqr += line.GetDirection()[y] * line.GetDirection()[y];
          float delta = line.GetDirection()[x] * minusExtents[x] +
                        line.GetDirection()[y] * tmp +
                        line.GetDirection()[z] * plusExtents[z];
          t = -delta / lSqr;
          distSq += minusExtents[x] * minusExtents[x] + tmp * tmp +
                    plusExtents[z] * plusExtents[z] + delta * t;
          pt[x] = extents[x];
          pt[y] = t - extents[y];
          pt[z] = -extents[z];
        } else {  // region 5 -- copied
          lSqr += line.GetDirection()[y] * line.GetDirection()[y];
          float delta = line.GetDirection()[x] * minusExtents[x] +
                        line.GetDirection()[y] * minusExtents[y] +
                        line.GetDirection()[z] * plusExtents[z];
          t = -delta / lSqr;
          distSq += minusExtents[x] * minusExtents[x] +
                    minusExtents[y] * minusExtents[y] +
                    plusExtents[z] * plusExtents[z] + delta * t;
          pt[x] = extents[x];
          pt[y] = extents[y];
          pt[z] = -extents[z];
        }
      }

      lSqr = line.GetDirection()[x] * line.GetDirection()[x] +
             line.GetDirection()[y] * line.GetDirection()[y];
      tmp = lSqr * plusExtents[z] -
            line.GetDirection()[z] * (line.GetDirection()[x] * minusExtents[x] +
                                      line.GetDirection()[y] * plusExtents[y]);

      if (tmp >= 0) {                        // region 1 or 2 -- copied
        if (tmp <= 2 * lSqr * extents[z]) {  // region 2 -- copied
          tmp = plusExtents[z] - (tmp / lSqr);
          lSqr += line.GetDirection()[z] * line.GetDirection()[z];
          float delta = line.GetDirection()[x] * minusExtents[x] +
                        line.GetDirection()[y] * plusExtents[y] +
                        line.GetDirection()[z] * tmp;
          t = -delta / lSqr;
          distSq += minusExtents[x] * minusExtents[x] +
                    minusExtents[y] * minusExtents[y] + tmp * tmp + delta * t;
          pt[x] = extents[x];
          pt[y] = -extents[y];
          pt[z] = t - extents[z];
        } else {  // region 1 -- copied
          lSqr += line.GetDirection()[z] * line.GetDirection()[z];
          float delta = line.GetDirection()[x] * minusExtents[x] +
                        line.GetDirection()[y] * minusExtents[y] +
                        line.GetDirection()[z] * plusExtents[z];
          t = -delta / lSqr;
          distSq += minusExtents[x] * minusExtents[x] +
                    plusExtents[y] * plusExtents[y] +
                    minusExtents[z] * minusExtents[z] + delta * t;
          pt[x] = extents[x];
          pt[y] = -extents[y];
          pt[z] = extents[z];
        }
        return pt;
      }
      // region 3
      lSqr += line.GetDirection()[y] * line.GetDirection()[y];
      float delta = line.GetDirection()[x] * minusExtents[x] +
                    line.GetDirection()[y] * plusExtents[y] +
                    line.GetDirection()[z] * plusExtents[z];
      t = -delta / lSqr;
      distSq += minusExtents[x] * minusExtents[x] +
                plusExtents[y] * plusExtents[y] +
                plusExtents[z] * plusExtents[z] + delta * t;
      pt[x] = extents[x];
      pt[y] = -extents[y];
      pt[z] = -extents[z];
      return pt;
    }
  }
  return pt;
}
bool CollisionsModule::CapsuleAABBIntersect(const Math::Vector3 &start,
                                            const Math::Vector3 &end,
                                            const float radius,
                                            const Math::Vector3 &extents,
                                            float *t) {
  // Does not work completely
  return false;
  // Youtube video on the topic:
  // https://www.youtube.com/watch?v=fa0gz0t61oA&t=971s
  // Youtube video on the
  // topic: https://www.youtube.com/watch?v=1OVZDeqkBvU&t=955s

  // Math::Vector3 to = end - start;
  // Math::Vector3 denom = 1.0f / to;
  // Math::Vector3 cneg = Math::Vector3::Scale(
  //    (-extents - radius * Math::Vector3::one - start), denom);
  // Math::Vector3 cpos = Math::Vector3::Scale(
  //    (extents + radius * Math::Vector3::one - start), denom);

  // Math::Vector3 ineg[Math::Vector3::ELEMENT_COUNT];
  // Math::Vector3 ipos[Math::Vector3::ELEMENT_COUNT];
  // for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
  //  ineg[i] = cneg[i] * to + start;
  //  ipos[i] = cpos[i] * to + start;
  //}

  // for (int x = 0; x < Math::Vector3::ELEMENT_COUNT; ++x) {
  //  int y = (x + 1) % 3;
  //  int z = (x + 2) % 3;
  //  if (Math::Util::Abs(ineg[x][y]) > extents[y] ||
  //      Math::Util::Abs(ineg[x][z]) > extents[z]) {
  //    cneg[x] = INFINITY;
  //  }
  //  if (Math::Util::Abs(ipos[x][y]) > extents[y] ||
  //      Math::Util::Abs(ipos[x][z]) > extents[z]) {
  //    cpos[x] = INFINITY;
  //  }
  //}

  // Math::Vector3 c;
  // float s;

  //// 8 points
  //// 12 lines
  //// 6 faces

  // float line[12];
  // float point[8];
  // Math::Vector3 face[4] = {Math::Vector3{1, 1, 0}, Math::Vector3{1, 0, 1},
  //                         Math::Vector3{0, 1, 1}, Math::Vector3::one};

  //// lines
  // line[0] = INFINITY;
  // c = Math::Vector3{-extents.x, extents.y, 0};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[0], &s))
  //  line[0] = s;

  // line[1] = INFINITY;
  // c = Math::Vector3{extents.x, extents.y, 0};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[0], &s))
  //  line[1] = s;

  // line[2] = INFINITY;
  // c = Math::Vector3{0, extents.y, -extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[2], &s))
  //  line[2] = s;

  // line[3] = INFINITY;
  // c = Math::Vector3{0, extents.y, extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[2], &s))
  //  line[3] = s;

  // line[4] = INFINITY;
  // c = Math::Vector3{-extents.x, -extents.y, 0};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[0], &s))
  //  line[4] = s;

  // line[5] = INFINITY;
  // c = Math::Vector3{extents.x, -extents.y, 0};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[0], &s))
  //  line[5] = s;

  // line[6] = INFINITY;
  // c = Math::Vector3{0, -extents.y, -extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[2], &s))
  //  line[6] = s;

  // line[7] = INFINITY;
  // c = Math::Vector3{0, -extents.y, extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[2], &s))
  //  line[7] = s;

  // line[8] = INFINITY;
  // c = Math::Vector3{extents.x, 0, extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[1], &s))
  //  line[8] = s;

  // line[9] = INFINITY;
  // c = Math::Vector3{extents.x, 0, -extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[1], &s))
  //  line[9] = s;

  // line[10] = INFINITY;
  // c = Math::Vector3{-extents.x, 0, extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[1], &s))
  //  line[10] = s;

  // line[11] = INFINITY;
  // c = Math::Vector3{-extents.x, 0, -extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[1], &s))
  //  line[11] = s;

  //// points
  // point[0] = INFINITY;
  // c = Math::Vector3{extents.x, extents.y, extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[3], &s))
  //  point[0] = s;

  // point[1] = INFINITY;
  // c = Math::Vector3{extents.x, extents.y, -extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[3], &s))
  //  point[1] = s;

  // point[2] = INFINITY;
  // c = Math::Vector3{extents.x, -extents.y, extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[3], &s))
  //  point[2] = s;

  // point[3] = INFINITY;
  // c = Math::Vector3{extents.x, -extents.y, -extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[3], &s))
  //  point[3] = s;

  // point[4] = INFINITY;
  // c = Math::Vector3{-extents.x, extents.y, extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[3], &s))
  //  point[4] = s;

  // point[5] = INFINITY;
  // c = Math::Vector3{-extents.x, extents.y, -extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[3], &s))
  //  point[5] = s;

  // point[6] = INFINITY;
  // c = Math::Vector3{-extents.x, -extents.y, extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[3], &s))
  //  point[6] = s;

  // point[7] = INFINITY;
  // c = Math::Vector3{-extents.x, -extents.y, -extents.z};
  // if (RaySphereIntersectLimited(start, end, c, radius, extents, face[3], &s))
  //  point[7] = s;

  //*t = Math::Util::Min(
  //    {cneg[0],  cneg[1],  cneg[2],  cpos[0],  cpos[1],  cpos[2],  line[0],
  //     line[1],  line[2],  line[3],  line[4],  line[5],  line[6],  line[7],
  //     line[8],  line[9],  line[10], line[11], point[0], point[1], point[2],
  //     point[3], point[4], point[5], point[6], point[7]});

  // return *t < INFINITY;
}
bool CollisionsModule::RaySphereIntersectLimited(
    const Math::Vector3 &start, const Math::Vector3 &end,
    const Math::Vector3 &center, float radius, const Math::Vector3 &extents,
    const Math::Vector3 &face, float *_t) {
  float &t = *_t;
  Math::Vector3 a = Math::Vector3::Scale(start, face) - center;
  Math::Vector3 b = Math::Vector3::Scale(end, face) - center;
  Math::Vector3 to = b - a;

  float toADot = Math::Vector3::Dot(to, a);
  float aSq = Math::Vector3::Dot(a, a);
  float toSq = Math::Vector3::Dot(to, to);

  float q = toADot * toADot - toSq * (aSq - radius * radius);
  if (q < 0) {
    t = 0;
    return false;
  } else {
    t = -(toADot + Math::Util::Sqrt(q)) / toSq;
    if (t < 0 || t > 1) {
      t = INFINITY;
      return false;
    }
    float d = SqDistanceToAABB(-extents, extents, to * t + start);
    if (Math::Util::Abs(d - radius * radius) > Math::Util::EPSILON) {
      t = INFINITY;
      return false;
    }
    return true;
  }
}
float CollisionsModule::SqDistanceToAABB(const Math::Vector3 &min,
                                         const Math::Vector3 &max,
                                         const Math::Vector3 &center) {
  float t, distSq = 0;
  for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; ++i) {
    if (center[i] < min[i]) {
      t = center[i] - min[i];
      distSq += t * t;
    } else if (center[i] > max[i]) {
      t = center[i] - max[i];
      distSq += t * t;
    }
  }
  return distSq;
}
Math::Vector3 CollisionsModule::ClosestPtSegmentOBB(const Math::Vector3 &p0,
                                                    const Math::Vector3 &p1,
                                                    const BoxCollider &box) {
  Math::Vector3 q0 = box.transform->LocalPosFromWorldPos(p0);
  Math::Vector3 q1 = box.transform->LocalPosFromWorldPos(p1);
  Line line = Line{q0, q1 - q0};
  float t, distSq;
  return ClosestPtLineOBB(line, box, &t, &distSq);
}
}  // namespace Isetta
