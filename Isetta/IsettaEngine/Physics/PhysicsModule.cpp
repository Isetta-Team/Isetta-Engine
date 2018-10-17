/*
 * Copyright (c) 2018 Isetta
 */
#include "Physics/PhysicsModule.h"

#include "Core/Debug/DebugDraw.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Matrix3.h"
#include "Core/Math/Vector3.h"
#include "Physics/AABB.h"
#include "Physics/Ray.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/Collider.h"
#include "Physics/SphereCollider.h"

namespace Isetta {
void Isetta::PhysicsModule::StartUp() { Collider::physicsModule = this; }

void PhysicsModule::Update(float deltaTime) {
  /*
   * TODO(Yidi)Broadphase check
   */
  // Collider obj1, obj2;
  // if (obj1.isTrigger && obj2.isTrigger) {
  //}  // TODO
  // else if (Intersection(obj1, obj2)) {
  //  if (obj1.isTrigger || obj2.isTrigger) {
  //    obj1.owner->OnTrigger(Collider::EntityKey{}, &obj2);
  //    obj2.owner->OnTrigger(Collider::EntityKey{}, &obj1);
  //  } else {
  //    obj1.owner->OnCollision(Collider::EntityKey{}, &obj2);
  //    obj2.owner->OnCollision(Collider::EntityKey{}, &obj1);
  //  }
  //}
  for (int i = 0; i < colliders.size(); i++) {
    if (colliders[i]->isStatic) continue;
    for (int j = 0; j < colliders.size(); j++) {
      if (colliders[i] == colliders[j]) continue;
      if (colliders[i]->Intersection(colliders[j])) {
        if (collisionPairs.find(CollisionPair(i, j)) != collisionPairs.end()) {
          // TODO(Jacob) Collision Stay
          OnCollisionStay(colliders[i]);
          OnCollisionStay(colliders[j]);
        } else {
          // TODO(Jacob) Collision Enter
          OnCollisionEnter(colliders[i]);
          OnCollisionEnter(colliders[j]);
          collisionPairs.insert(CollisionPair(i, j));
          // TODO(Jacob) remove
          if (collisions.find(i) != collisions.end()) {
            collisions[i]++;
          } else {
            collisions.insert(std::make_pair(i, 1));
          }
          if (collisions.find(j) != collisions.end()) {
            collisions[j]++;
          } else {
            collisions.insert(std::make_pair(j, 1));
          }
          //
        }
        colliders[i]->debugColor = Color::red;
        colliders[j]->debugColor = Color::red;
      } else if (collisionPairs.find(CollisionPair(i, j)) !=
                 collisionPairs.end()) {
        // TODO(Jacob) Collision Exit
        OnCollisionExit(colliders[i]);
        OnCollisionExit(colliders[j]);
        collisionPairs.erase(CollisionPair(i, j));
        // TODO(Jacob) remove
        collisions[i]--;
        collisions[j]--;
        if (collisions[i] == 0) {
          colliders[i]->debugColor = Color::green;
        }
        if (collisions[j] == 0) {
          colliders[j]->debugColor = Color::green;
        }
        //
      }
    }
  }
}

void PhysicsModule::ShutDown() {}

void PhysicsModule::OnCollisionEnter(Collider *const collider) {
  for (auto component : collider->entity->GetComponents<Component>()) {
    component->OnDestroy();
  }
}

void PhysicsModule::OnCollisionStay(Collider *const collider) {
  for (auto component : collider->entity->GetComponents<Component>()) {
    component->OnDestroy();
  }
}

void PhysicsModule::OnCollisionExit(Collider *const collider) {
  for (auto component : collider->entity->GetComponents<Component>()) {
    component->OnDestroy();
  }
}

bool PhysicsModule::Intersection(const BoxCollider &a, const BoxCollider &b) {
  float ra, rb;
  Math::Matrix3 rot, absRot;

  // TODO(all) is there a better way to do this?
  for (int i = 0; i < Math::Matrix3::ROW_COUNT; i++) {
    for (int j = 0; j < Math::Matrix3::ROW_COUNT; j++) {
      rot[i][j] = Math::Vector3::Dot(a.GetTransform().GetAxis(i),
                                     b.GetTransform().GetAxis(j));
    }
  }
  Math::Vector3 t = b.GetWorldCenter() - a.GetWorldCenter();
  t = Math::Vector3{Math::Vector3::Dot(t, a.GetTransform().GetLeft()),
                    Math::Vector3::Dot(t, a.GetTransform().GetUp()),
                    Math::Vector3::Dot(t, a.GetTransform().GetForward())};
  for (int i = 0; i < Math::Matrix3::ROW_COUNT; i++) {
    for (int j = 0; j < Math::Matrix3::ROW_COUNT; j++) {
      absRot[i][j] = Math::Util::Abs(rot[i][j]) + Math::Util::EPSILON;
    }
  }

  Math::Vector3 aExtents = 0.5f * a.GetWorldSize();
  Math::Vector3 bExtents = 0.5f * b.GetWorldSize();
  // Test axes L = A0, L = A1, L = A2
  for (int i = 0; i < 3; i++) {
    ra = aExtents[i];
    rb = bExtents[0] * absRot[i][0] + bExtents[1] * absRot[i][1] +
         bExtents[2] * absRot[i][2];
    if (Math::Util::Abs(t[i]) > ra + rb) return 0;
  }
  // Test axes L = B0, L = B1, L = B2
  for (int i = 0; i < 3; i++) {
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

bool PhysicsModule::Intersection(const BoxCollider &box,
                                 const SphereCollider &sphere) {
  Math::Vector3 center = sphere.GetWorldCenter();
  Math::Vector3 pt = ClosestPtPointOBB(center, box);
  Math::Vector3 to = pt - center;
  return Math::Vector3::Dot(to, to) <=
         sphere.GetWorldRadius() * sphere.GetWorldRadius();
}
bool PhysicsModule::Intersection(const BoxCollider &box,
                                 const CapsuleCollider &capsule) {
  return false;
  Math::Matrix4 rot, scale;
  float radiusScale = capsule.GetWorldCapsule(&rot, &scale);
  Math::Vector3 dir = (Math::Vector3)(
      rot * scale *
      Math::Matrix4::Scale(Math::Vector3{capsule.height - 2 * capsule.radius}) *
      Math::Vector4{0, 1, 0, 0});
  Math::Vector3 p0 = capsule.GetWorldCenter() - dir;
  Math::Vector3 p1 = capsule.GetWorldCenter() + dir;

  return SqDistSegmentOBB(p0, p1, box) <=
         Math::Util::Square(capsule.radius * radiusScale);
}
bool PhysicsModule::Intersection(const SphereCollider &a,
                                 const SphereCollider &b) {
  return (a.GetWorldCenter() - b.GetWorldCenter()).SqrMagnitude() <=
         Math::Util::Square(a.GetWorldRadius() + b.GetWorldRadius());
}
bool PhysicsModule::Intersection(const SphereCollider &sphere,
                                 const BoxCollider &box) {
  return Intersection(box, sphere);
}
bool PhysicsModule::Intersection(const SphereCollider &sphere,
                                 const CapsuleCollider &capsule) {
  Math::Matrix4 rot, scale;
  float radiusScale = capsule.GetWorldCapsule(&rot, &scale);
  Math::Vector3 dir = (Math::Vector3)(
      rot * scale *
      Math::Matrix4::Scale(Math::Vector3{capsule.height - 2 * capsule.radius}) *
      Math::Vector4{0, 1, 0, 0});
  Math::Vector3 p0 = capsule.GetWorldCenter() - dir;
  Math::Vector3 p1 = capsule.GetWorldCenter() + dir;
  float distSq = SqDistPointSegment(p0, p1, sphere.GetWorldCenter());
  return distSq <= Math::Util::Square(sphere.radius * sphere.GetWorldRadius() +
                                      capsule.radius * radiusScale);
}
bool PhysicsModule::Intersection(const CapsuleCollider &a,
                                 const CapsuleCollider &b) {
  Math::Matrix4 aRot, aScale, bRot, bScale;
  float arScale = a.GetWorldCapsule(&aRot, &aScale);
  float brScale = b.GetWorldCapsule(&bRot, &bScale);
  Math::Vector3 aDir = (Math::Vector3)(
      aRot * aScale *
      Math::Matrix4::Scale(Math::Vector3{a.height - 2 * a.radius}) *
      Math::Vector4{0, 1, 0, 0});
  Math::Vector3 bDir = (Math::Vector3)(
      bRot * bScale *
      Math::Matrix4::Scale(Math::Vector3{b.height - 2 * b.radius}) *
      Math::Vector4{0, 1, 0, 0});
  Math::Vector3 aP0 = a.GetWorldCenter() - aDir;
  Math::Vector3 aP1 = a.GetWorldCenter() + aDir;
  Math::Vector3 bP0 = b.GetWorldCenter() - bDir;
  Math::Vector3 bP1 = b.GetWorldCenter() + bDir;

  float s, t;
  Math::Vector3 ac, bc;
  float distSq = ClosestPtSegmentSegment(aP0, aP1, bP0, bP1, &s, &t, &ac, &bc);

  return distSq <= Math::Util::Square(a.radius * arScale + b.radius * brScale);
}
bool PhysicsModule::Intersection(const CapsuleCollider &capsule,
                                 const BoxCollider &box) {
  return Intersection(box, capsule);
}
bool PhysicsModule::Intersection(const CapsuleCollider &capsule,
                                 const SphereCollider &sphere) {
  return Intersection(sphere, capsule);
}
bool PhysicsModule::Intersection(const Math::Vector3 &p0,
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
float PhysicsModule::SqDistPointSegment(const Math::Vector3 &a,
                                        const Math::Vector3 &b,
                                        const Math::Vector3 &c) {
  Math::Vector3 ab = b - a, ac = c - a, bc = c - b;
  float e = Math::Vector3::Dot(ac, ab);
  if (e <= 0) return Math::Vector3::Dot(ac, ac);
  float f = Math::Vector3::Dot(ab, ab);
  if (e >= f) return Math::Vector3::Dot(bc, bc);
  return Math::Vector3::Dot(ac, ac) - e * e / f;
}
float PhysicsModule::SqDistPointOBB(const Math::Vector3 &point,
                                    const BoxCollider &box) {
  Math::Vector3 closest = ClosestPtPointOBB(point, box);
  // DebugDraw::Point(closest, Color::blue, 20, 0.1, false);
  float sqDist = Math::Vector3::Dot(closest - point, closest - point);
  return sqDist;
}
float PhysicsModule::SqDistSegmentOBB(const Math::Vector3 &p0,
                                      const Math::Vector3 &p1,
                                      const BoxCollider &box) {
  Ray ray = Ray{p0, p1 - p0};
  // Ray ray = Ray{q0, q1 - q0};
  float t, distSq;
  Math::Vector3 pt = ClosestPtRayOBB(ray, box, &t, &distSq);
  DebugDraw::Point(pt, Color::red, 20, 0.1, false);
  LOG_INFO(Debug::Channel::Physics, "(%f)", t);
  // LOG_INFO(Debug::Channel::Physics, "(%f, %f, %f)", pt.x, pt.y, pt.z);
  if (t < 0) {
    distSq = SqDistPointOBB(p0, box);
    DebugDraw::Point(p0, Color::white, 20, 0.1, false);
  } else if (t > 1) {
    distSq = SqDistPointOBB(p1, box);
    DebugDraw::Point(p1, Color::white, 20, 0.1, false);
  }
  return distSq;
}
Math::Vector3 PhysicsModule::ClosestPtPointSegment(const Math::Vector3 &point,
                                                   const Math::Vector3 &p0,
                                                   const Math::Vector3 &p1,
                                                   float *const _t) {
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
float PhysicsModule::ClosestPtSegmentSegment(
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
Math::Vector3 PhysicsModule::ClossetPtPointAABB(const Math::Vector3 &point,
                                                const AABB &aabb) {
  Math::Vector3 pt;
  pt.x = Math::Util::Min(Math::Util::Max(point.x, aabb.GetMin().x),
                         aabb.GetMax().x);
  pt.y = Math::Util::Min(Math::Util::Max(point.y, aabb.GetMin().y),
                         aabb.GetMax().y);
  pt.z = Math::Util::Min(Math::Util::Max(point.z, aabb.GetMin().z),
                         aabb.GetMax().z);
  return Math::Vector3();
}
Math::Vector3 PhysicsModule::ClosestPtPointOBB(const Math::Vector3 &point,
                                               const BoxCollider &box) {
  Math::Vector3 d = point - (box.GetWorldCenter());
  Math::Vector3 pt = box.GetWorldCenter();

  Math::Vector3 extents =
      0.5f * Math::Vector3::Scale(box.GetTransform().GetWorldScale(), box.size);
  for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; i++) {
    float dist = Math::Vector3::Dot(d, box.GetTransform().GetAxis(i));
    if (dist > extents[i]) {
      dist = extents[i];
    }
    if (dist < -extents[i]) {
      dist = -extents[i];
    }
    pt += dist * box.GetTransform().GetAxis(i);
  }
  return pt;
}
Math::Vector3 PhysicsModule::ClosestPtRayOBB(const Ray &ray,
                                             const BoxCollider &box, float *_t,
                                             float *_distSq) {
  Math::Vector3 o = box.GetTransform().LocalPosFromWorldPos(ray.GetOrigin());
  // Math::Vector3 o = ray.GetOrigin();
  Math::Vector3 dir =
      box.GetTransform().LocalDirFromWorldDir(ray.GetDirection());
  // Math::Vector3 dir = ray.GetDirection();
  float &t = *_t;
  float &distSq = *_distSq;
  distSq = 0;

  int perp = 0;
  for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; i++) {
    if (Math::Vector3::Dot(dir, box.GetTransform().GetAxis(i)) <
        Math::Util::EPSILON)
      perp++;
  }
  switch (perp) {
    case 3:
      LOG_INFO(Debug::Channel::Physics, "Case 3");
      distSq = t = 0;
      return ClosestPtPointOBB(ray.GetOrigin(), box);
    case 2: {
      LOG_INFO(Debug::Channel::Physics, "Case 2");
      int x = -1;
      for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; i++) {
        if (Math::Vector3::Dot(dir, box.GetTransform().GetAxis(i)) >
            Math::Util::EPSILON) {
          x = i;
          break;
        }
      }
      int y = (x + 1) % Math::Vector3::ELEMENT_COUNT,
          z = (x + 2) % Math::Vector3::ELEMENT_COUNT;

      Math::Vector3 extents = 0.5f * box.GetWorldSize();
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
        o[y] = extents[y];
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
      return box.GetTransform().WorldPosFromLocalPos(pt);  // TODO test
      // return pt;
      // +box.GetWorldCenter();
      // return pt + box.GetWorldCenter();
    }
    case 1: {
      LOG_INFO(Debug::Channel::Physics, "Case 1");
      int z = -1;
      for (int i = 0; i < Math::Vector3::ELEMENT_COUNT; i++) {
        if (Math::Vector3::Dot(dir, box.GetTransform().GetAxis(i)) <
            Math::Util::EPSILON) {
          z = i;
          break;
        }
      }
      int x = (z + 1) % Math::Vector3::ELEMENT_COUNT,
          y = (z + 2) % Math::Vector3::ELEMENT_COUNT;

      Math::Vector3 extents = 0.5f * box.GetWorldSize();
      Math::Vector3 minusExtents = o - extents;
      float p0 = dir[y] * minusExtents[x];
      float p1 = dir[x] * minusExtents[y];
      Math::Vector3 pt;
      pt[z] = o[z];
      if (p0 >= p1) {
        // x-axis
        pt[x] = extents[x];
        float tmp = o[y] + extents[y];
        float delta = p0 - dir[x] * tmp;
        if (delta >= 0) {
          // no intersection
          float invLSq = 1.0f / (dir[x] * dir[x] + dir[y] * dir[y]);
          distSq += delta * delta * invLSq;

          pt[y] = -extents[y];
          t = -(dir[x] * minusExtents[x] * dir[y] + tmp) * invLSq;
        } else {
          float inv = 1.0f / dir[x];
          pt[y] = o[y] - (p0 * inv);  // TODO might be wrong
          t = -minusExtents[x] * inv;
          distSq = 0;
        }
      } else {
        // y-axis
        pt[y] = extents[y];
        float tmp = o[x] + extents[x];
        float delta = p1 - dir[y] * tmp;
        if (delta >= 0) {
          // no intersection
          float invLSq = 1.0f / (dir[x] * dir[x] + dir[y] * dir[y]);
          distSq += delta * delta * invLSq;

          pt[y] = -extents[y];
          t = -(dir[y] * minusExtents[y] * dir[x] + tmp) * invLSq;
        } else {
          float inv = 1.0f / dir[y];
          pt[y] = o[y] - (p0 * inv);  // TODO might be wrong
          t = -minusExtents[y] * inv;
          distSq = 0;
        }
      }

      if (o[z] < -extents[z]) {
        float delta = o[z] + extents[z];
        distSq += delta * delta;
        pt[z] = -extents.z;
      } else if (o[z] > extents[z]) {
        float delta = o[z] - extents[z];
        distSq += delta * delta;
        pt[z] = extents[z];
      }

      DebugDraw::Point(pt, Color::brown, 10, .1, false);
      // distSq
      return box.GetTransform().WorldPosFromLocalPos(pt);
      // return pt;
    }
    case 0: {
      LOG_INFO(Debug::Channel::Physics, "Case 0");
      Math::Vector3 extents = 0.5f * box.GetWorldSize();
      Math::Vector3 minusExtents = o - extents;
      float dyEx = dir.y * minusExtents.x;
      float dxEy = dir.x * minusExtents.y;

      Math::Vector3 pt;
      Ray r = Ray{o, dir};
      if (dyEx >= dxEy) {
        float dzEx = dir.z * minusExtents.x;
        float dxEz = dir.x * minusExtents.z;

        if (dzEx >= dxEz) {
          pt = Face(0, r, box, minusExtents, &t, &distSq);
        } else {
          pt = Face(2, r, box, minusExtents, &t, &distSq);
        }
      } else {
        float dzEy = dir.z * minusExtents.y;
        float dyEz = dir.y * minusExtents.z;

        if (dzEy >= dyEz) {
          pt = Face(1, r, box, minusExtents, &t, &distSq);
        } else {
          pt = Face(2, r, box, minusExtents, &t, &distSq);
        }
      }
      // distSq;
      return pt;
    }
  };
}  // namespace Isetta
Math::Vector3 PhysicsModule::Face(int x, const Ray &ray, const BoxCollider &box,
                                  const Math::Vector3 &minusExtents, float *_t,
                                  float *_distSq) {
  int y = (x + 1) % Math::Vector3::ELEMENT_COUNT,
      z = (x + 2) % Math::Vector3::ELEMENT_COUNT;
  float &t = *_t, &distSq = *_distSq;
  Math::Vector3 pt = ray.GetOrigin();
  distSq = 0;

  Math::Vector3 extents = 0.5 * box.GetWorldSize();
  Math::Vector3 plusExtents = ray.GetOrigin() + extents;

  if (ray.GetDirection()[x] * plusExtents[y] >=
      ray.GetDirection()[y] * minusExtents[x]) {  // region 0, 5, or 4
    if (ray.GetDirection().x * plusExtents[z] >=
        ray.GetDirection()[z] * minusExtents[x]) {  // region 0
      pt[x] = extents[x];
      float inv = 1.0f / ray.GetDirection()[x];
      pt[y] -= ray.GetDirection()[y] * minusExtents[x] * inv;
      pt[z] -= ray.GetDirection()[z] * minusExtents[x] * inv;
      t = -minusExtents[x] * inv;  // todo what?
    } else {                       // region 4 or 5
      float lSqr = ray.GetDirection()[x] * ray.GetDirection()[x] +
                   ray.GetDirection()[z] * ray.GetDirection()[z];
      float tmp =
          lSqr * plusExtents[y] -
          ray.GetDirection()[y] * (ray.GetDirection()[x] * minusExtents[x] +
                                   ray.GetDirection()[z] * plusExtents[z]);
      if (tmp <= 2 * lSqr * extents[y]) {  // region 4
        tmp = plusExtents[y] - (tmp / lSqr);
        lSqr += ray.GetDirection()[y] * ray.GetDirection()[y];
        float delta = ray.GetDirection()[x] * minusExtents[x] +
                      ray.GetDirection()[y] * tmp +
                      ray.GetDirection()[z] * plusExtents[z];
        t = -delta / lSqr;
        distSq += minusExtents[x] * minusExtents[x] + tmp * tmp +
                  plusExtents[z] * plusExtents[z] + delta * t;
        pt[x] = extents[x];
        pt[y] = t - extents[y];
        pt[z] = -extents[z];
      } else {  // region 5
        lSqr += ray.GetDirection()[y] * ray.GetDirection()[y];
        float delta = ray.GetDirection()[x] * minusExtents[x] +
                      ray.GetDirection()[y] * minusExtents[y] +
                      ray.GetDirection()[z] * plusExtents[z];
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
    if (ray.GetDirection()[x] * plusExtents[z] >=
        ray.GetDirection()[z] * minusExtents[x]) {
      float lSqr = ray.GetDirection()[x] * ray.GetDirection()[x] +
                   ray.GetDirection()[y] * ray.GetDirection()[y];
      float tmp =
          lSqr * plusExtents[z] -
          ray.GetDirection()[z] * (ray.GetDirection()[x] * minusExtents[x] +
                                   ray.GetDirection()[y] * plusExtents[y]);
      if (tmp <= 2 * lSqr * extents[z]) {  // region 2
        tmp = plusExtents[z] - (tmp / lSqr);
        lSqr += ray.GetDirection()[z] * ray.GetDirection()[z];
        float delta = ray.GetDirection()[x] * minusExtents[x] +
                      ray.GetDirection()[y] * plusExtents[y] +
                      ray.GetDirection()[z] * tmp;
        t = -delta / lSqr;
        distSq += minusExtents[x] * minusExtents[x] +
                  minusExtents[y] * minusExtents[y] + tmp * tmp + delta * t;
        pt[x] = extents[x];
        pt[y] = -extents[y];
        pt[z] = -extents[z];
      } else {  // region 1
        lSqr += ray.GetDirection()[z] * ray.GetDirection()[z];
        float delta = ray.GetDirection()[x] * minusExtents[x] +
                      ray.GetDirection()[y] * minusExtents[y] +
                      ray.GetDirection()[z] * plusExtents[z];
        t = -delta / lSqr;
        distSq += minusExtents[x] * minusExtents[x] +
                  plusExtents[y] * plusExtents[y] +
                  minusExtents[z] * minusExtents[z] + delta * t;
        pt[x] = extents[x];
        pt[y] = -extents[y];
        pt[z] = extents[z];
      }
    } else {
      float lSqr = ray.GetDirection()[x] * ray.GetDirection()[x] +
                   ray.GetDirection()[z] * ray.GetDirection()[z];
      float tmp =
          lSqr * plusExtents[y] -
          ray.GetDirection()[y] * (ray.GetDirection()[x] * minusExtents[x] +
                                   ray.GetDirection()[z] * plusExtents[z]);
      if (tmp >= 0) {                        // region 4 or 5 -- copied
        if (tmp <= 2 * lSqr * extents[z]) {  // region 4 -- copied
          tmp = plusExtents[y] - (tmp / lSqr);
          lSqr += ray.GetDirection()[y] * ray.GetDirection()[y];
          float delta = ray.GetDirection()[x] * minusExtents[x] +
                        ray.GetDirection()[y] * tmp +
                        ray.GetDirection()[z] * plusExtents[z];
          t = -delta / lSqr;
          distSq += minusExtents[x] * minusExtents[x] + tmp * tmp +
                    plusExtents[z] * plusExtents[z] + delta * t;
          pt[x] = extents[x];
          pt[y] = t - extents[y];
          pt[z] = -extents[z];
        } else {  // region 5 -- copied
          lSqr += ray.GetDirection()[y] * ray.GetDirection()[y];
          float delta = ray.GetDirection()[x] * minusExtents[x] +
                        ray.GetDirection()[y] * minusExtents[y] +
                        ray.GetDirection()[z] * plusExtents[z];
          t = -delta / lSqr;
          distSq += minusExtents[x] * minusExtents[x] +
                    minusExtents[y] * minusExtents[y] +
                    plusExtents[z] * plusExtents[z] + delta * t;
          pt[x] = extents[x];
          pt[y] = extents[y];
          pt[z] = -extents[z];
        }
      }

      lSqr = ray.GetDirection()[x] * ray.GetDirection()[x] +
             ray.GetDirection()[y] * ray.GetDirection()[y];
      tmp = lSqr * plusExtents[z] -
            ray.GetDirection()[z] * (ray.GetDirection()[x] * minusExtents[x] +
                                     ray.GetDirection()[y] * plusExtents[y]);

      if (tmp >= 0) {                        // region 1 or 2 -- copied
        if (tmp <= 2 * lSqr * extents[z]) {  // region 2 -- copied
          tmp = plusExtents[z] - (tmp / lSqr);
          lSqr += ray.GetDirection()[z] * ray.GetDirection()[z];
          float delta = ray.GetDirection()[x] * minusExtents[x] +
                        ray.GetDirection()[y] * plusExtents[y] +
                        ray.GetDirection()[z] * tmp;
          t = -delta / lSqr;
          distSq += minusExtents[x] * minusExtents[x] +
                    minusExtents[y] * minusExtents[y] + tmp * tmp + delta * t;
          pt[x] = extents[x];
          pt[y] = -extents[y];
          pt[z] = -extents[z];
        } else {  // region 1 -- copied
          lSqr += ray.GetDirection()[z] * ray.GetDirection()[z];
          float delta = ray.GetDirection()[x] * minusExtents[x] +
                        ray.GetDirection()[y] * minusExtents[y] +
                        ray.GetDirection()[z] * plusExtents[z];
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
      lSqr += ray.GetDirection()[y] * ray.GetDirection()[y];
      float delta = ray.GetDirection()[x] * minusExtents[x] +
                    ray.GetDirection()[y] * plusExtents[y] +
                    ray.GetDirection()[z] * plusExtents[z];
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
  return Math::Vector3();
}
Math::Vector3 PhysicsModule::ClosestPtSegmentOBB(const Math::Vector3 &p0,
                                                 const Math::Vector3 &p1,
                                                 const BoxCollider &box) {
  Math::Vector3 q0 = box.GetTransform().LocalPosFromWorldPos(p0);
  Math::Vector3 q1 = box.GetTransform().LocalPosFromWorldPos(p1);
  Ray ray = Ray{q0, q1 - q0};
  float t, distSq;
  return ClosestPtRayOBB(ray, box, &t, &distSq);
}
}  // namespace Isetta
