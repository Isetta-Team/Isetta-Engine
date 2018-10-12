/*
 * Copyright (c) 2018 Isetta
 */
#include "Physics/PhysicsModule.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Matrix3.h"
#include "Physics/AABB.h"
#include "Scene/Transform.h"

#include "Physics/BoxCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/Collider.h"
#include "Physics/SphereCollider.h"

namespace Isetta {
void Isetta::PhysicsModule::StartUp() { Collider::physicsModule = this; }

void PhysicsModule::Update(float deltaTime) {}

void PhysicsModule::ShutDown() {}

// TODO(Jacob) Maybe have this implemented?
// bool SeparatingPlane(...)
// struct OBB {
//  Math::Matrix3 axis;
//  Math::Vector3 center;
//  Math::Vector3 halfScale;
//};

bool PhysicsModule::Intersection(const BoxCollider &a, const BoxCollider &b) {
  float ra, rb;
  Math::Matrix3 rot, absRot;

  // TODO(all) is there a better way to do this?
  for (int i = 0; i < Math::Matrix3::ELEMENT_COUNT; i++) {
    for (int j = 0; j < Math::Matrix3::ELEMENT_COUNT; j++) {
      rot[i][j] = Math::Vector3::Dot(a.GetTransform().GetAxis(i),
                                     b.GetTransform().GetAxis(j));
    }
  }
  Math::Vector3 t = b.center - a.center;
  t = Math::Vector3{Math::Vector3::Dot(t, a.GetTransform().GetLeft()),
                    Math::Vector3::Dot(t, a.GetTransform().GetUp()),
                    Math::Vector3::Dot(t, a.GetTransform().GetForward())};
  for (int i = 0; i < Math::Matrix3::ELEMENT_COUNT; i++) {
    for (int j = 0; j < Math::Matrix3::ELEMENT_COUNT; j++) {
      absRot[i][j] = Math::Util::Abs(rot[i][j]) + Math::Util::EPSILON;
    }
  }

  Math::Vector3 aExtents =
      0.5f * Math::Vector3::Scale(a.GetTransform().GetWorldScale(), a.size);
  Math::Vector3 bExtents =
      0.5f * Math::Vector3::Scale(b.GetTransform().GetWorldScale(), b.size);
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
  Math::Vector3 center = sphere.GetTransform().GetWorldPos() + sphere.center;
  Math::Vector3 pt = ClosestPtPointOBB(center, box);
  Math::Vector3 to = pt - center;
  return Math::Vector3::Dot(pt, pt) <=
         Math::Util::Square(sphere.radius *
                            sphere.GetTransform().GetWorldScale().Max());
}
bool PhysicsModule::Intersection(const BoxCollider &, const CapsuleCollider &) {
  return false;
}
bool PhysicsModule::Intersection(const SphereCollider &a,
                                 const SphereCollider &b) {
  return ((a.GetTransform().GetWorldPos() + a.center) -
          (b.GetTransform().GetWorldPos() + b.center))
             .SqrMagnitude() <=
         Math::Util::Square(a.radius * a.GetTransform().GetWorldScale().Max() +
                            b.radius * b.GetTransform().GetWorldScale().Max());
}
bool PhysicsModule::Intersection(const SphereCollider &sphere,
                                 const BoxCollider &box) {
  return Intersection(box, sphere);
}
bool PhysicsModule::Intersection(const SphereCollider &sphere,
                                 const CapsuleCollider &capsule) {
  Math::Matrix4 rot, scale;
  float radiusScale = capsule.TransformUtility(rot, scale);
  Math::Vector3 dir = (Math::Vector3)(
      rot * scale *
      Math::Matrix4::Scale(Math::Vector3{capsule.height - 2 * capsule.radius}) *
      Math::Vector4{0, 1, 0, 0});
  Math::Vector3 p0 =
      capsule.GetTransform().GetWorldPos() + capsule.center - 0.5 * dir;
  Math::Vector3 p1 =
      capsule.GetTransform().GetWorldPos() + capsule.center + 0.5 * dir;
  float distSq = SqDistPointSegment(
      p0, p1, sphere.GetTransform().GetWorldPos() + sphere.center);
  return distSq <=
         Math::Util::Square(sphere.radius *
                                sphere.GetTransform().GetWorldScale().Max() +
                            capsule.radius * radiusScale);
  // Math::Vector3 to = sphere.GetTransform().GetWorldPos() + sphere.center -
  // p0; float projection =
  //    Math::Util::Square(Math::Vector3::Dot(to, dir)) / dir.SqrMagnitude();
  // projection = Math::Util::Min(projection, capsule.height);
  // return to.SqrMagnitude() - projection <=
  //       Math::Util::Square(sphere.radius *
  //                              sphere.GetTransform().GetWorldScale().Max()
  //                              +
  //                          capsule.radius * radiusScale);
}
bool PhysicsModule::Intersection(const CapsuleCollider &a,
                                 const CapsuleCollider &b) {
  float s, t;
  Math::Vector3 ac, bc;
  Math::Matrix4 aRot, aScale, bRot, bScale;
  float arScale = a.TransformUtility(aRot, aScale);
  float brScale = b.TransformUtility(bRot, bScale);
  Math::Vector3 aDir = (Math::Vector3)(
      aRot * aScale *
      Math::Matrix4::Scale(Math::Vector3{a.height - 2 * a.radius}) *
      Math::Vector4{0, 1, 0, 0});
  Math::Vector3 bDir = (Math::Vector3)(
      bRot * bScale *
      Math::Matrix4::Scale(Math::Vector3{b.height - 2 * b.radius}) *
      Math::Vector4{0, 1, 0, 0});
  Math::Vector3 aP0 = a.GetTransform().GetWorldPos() + a.center - 0.5 * aDir;
  Math::Vector3 aP1 = a.GetTransform().GetWorldPos() + a.center + 0.5 * aDir;
  Math::Vector3 bP0 = b.GetTransform().GetWorldPos() + b.center - 0.5 * bDir;
  Math::Vector3 bP1 = b.GetTransform().GetWorldPos() + b.center + 0.5 * bDir;

  float distSq = ClosestPointSegmentSegment(aP0, bP0, aP1, bP1, s, t, ac, bc);
  return distSq <= Math::Util::Square(a.radius * arScale + b.radius * brScale);
  // Math::Matrix4 aRot, aScale, bRot, bScale;
  // float arScale = a.TransformUtility(aRot, aScale);
  // float brScale = b.TransformUtility(bRot, bScale);
  // Math::Vector3 aDir = (Math::Vector3)(
  //    aRot * aScale *
  //    Math::Matrix4::Scale(Math::Vector3{a.height - 2 * a.radius}) *
  //    Math::Vector4{0, 1, 0, 0});
  // Math::Vector3 bDir = (Math::Vector3)(
  //    bRot * bScale *
  //    Math::Matrix4::Scale(Math::Vector3{b.height - 2 * b.radius}) *
  //    Math::Vector4{0, 1, 0, 0});
  // Math::Vector3 aP0 = a.GetTransform().GetWorldPos() + a.center - 0.5 *
  // aDir; Math::Vector3 bP0 = b.GetTransform().GetWorldPos() + b.center - 0.5
  // * bDir;

  // float abDir = Math::Vector3::Dot(aDir, bDir);
  // float abDirSq = abDir * abDir;
  // float aDirSq = aDir.SqrMagnitude();
  // float bDirSq = bDir.SqrMagnitude();

  // float distSq;
  // if (Math::Util::FuzzyEquals(abDirSq, aDirSq * bDirSq)) {
  //  // TODO parallel lines
  //  // distSq = (P0 - P1).Sq
  //} else {
  //  Math::Vector3 diff = bP0 - aP0;
  //  float aDotDiff = Math::Vector3::Dot(aDir, diff);
  //  float bDotDiff = Math::Vector3::Dot(bDir, diff);
  //  float div = abDirSq - aDirSq * bDirSq;
  //  float at = (-bDirSq * aDotDiff + abDir * bDotDiff) * div;
  //  at = Math::Util::Max(at, a.height);
  //  float bt = (-abDir * aDotDiff + aDirSq * bDotDiff) * div;
  //  bt = Math::Util::Max(bt, b.height);
  //  distSq = ((aP0 + at * aDir) - (bP0 + bt * bDir)).SqrMagnitude();
  //}

  // return distSq <= Math::Util::Square(a.radius * arScale + b.radius *
  // brScale);
}
bool PhysicsModule::Intersection(const CapsuleCollider &capsule,
                                 const BoxCollider &box) {
  return Intersection(box, capsule);
}
bool PhysicsModule::Intersection(const CapsuleCollider &capsule,
                                 const SphereCollider &sphere) {
  return Intersection(sphere, capsule);
}
float PhysicsModule::SqDistPointSegment(const Math::Vector3 &a,
                                        const Math::Vector3 &b,
                                        const Math::Vector3 &c) {
  Math::Vector3 ab = b - a, ac = c - a, bc = c - b;
  float e = Math::Vector3::Dot(ac, ab);
  if (e <= 0) {
    return Math::Vector3::Dot(ac, ac);
  }
  float f = Math::Vector3::Dot(ab, ab);
  if (e >= f) {
    return Math::Vector3::Dot(bc, bc);
  }
  return Math::Vector3::Dot(ac, ac) - e * e / f;
}
float PhysicsModule::ClosestPointSegmentSegment(
    const Math::Vector3 &p1, const Math::Vector3 &q1, const Math::Vector3 &p2,
    const Math::Vector3 &q2, float &s, float &t, Math::Vector3 &c1,
    Math::Vector3 &c2) {
  Math::Vector3 d1 = q1 - p1;  // Direction vector of segment S1
  Math::Vector3 d2 = q2 - p2;  // Direction vector of segment S2
  Math::Vector3 r = p1 - p2;
  float a = Math::Vector3::Dot(
      d1, d1);  // Squared length of segment S1, always nonnegative
  float e = Math::Vector3::Dot(
      d2, d2);  // Squared length of segment S2, always nonnegative
  float f = Math::Vector3::Dot(d2, r);
  // Check if either or both segments degenerate into points
  if (a <= Math::Util::EPSILON && e <= Math::Util::EPSILON) {
    // Both segments degenerate into points
    s = t = 0.0f;
    c1 = p1;
    c2 = p2;
    return Math::Vector3::Dot(c1 - c2, c1 - c2);
  }
  if (a <= Math::Util::EPSILON) {
    // First segment degenerates into a point
    s = 0.0f;
    t = f / e;  // s = 0 => t = (b*s + f) / e = f / e
    t = Math::Util::Clamp(t, 0.0f, 1.0f);
  } else {
    float c = Math::Vector3::Dot(d1, r);
    if (e <= Math::Util::EPSILON) {
      // Second segment degenerates into a point
      t = 0.0f;
      s = Math::Util::Clamp(-c / a, 0.0f,
                            1.0f);  // t = 0 => s = (b*t - c) / a = -c / a
    } else {
      // The general nondegenerate case starts here
      float b = Math::Vector3::Dot(d1, d2);
      float denom = a * e - b * b;  // Always nonnegative
                                    // If segments not parallel, compute closest
                                    // point on L1 to L2 and clamp to segment
                                    // S1. Else pick arbitrary s (here 0)
      if (denom != 0.0f) {
        s = Math::Util::Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
      } else
        s = 0.0f;
      // Compute point on L2 closest to S1(s) using
      // t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
      t = (b * s + f) / e;
      // If t in [0,1] done. Else clamp t, recompute s for the new value
      // of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
      // and clamp s to [0, 1]
      if (t < 0.0f) {
        t = 0.0f;
        s = Math::Util::Clamp(-c / a, 0.0f, 1.0f);
      } else if (t > 1.0f) {
        t = 1.0f;
        s = Math::Util::Clamp((b - c) / a, 0.0f, 1.0f);
      }
    }
  }
  c1 = p1 + d1 * s;
  c2 = p2 + d2 * t;
  return Math::Vector3::Dot(c1 - c2, c1 - c2);
}
Math::Vector3 PhysicsModule::ClosestPtPointOBB(const Math::Vector3 &point,
                                               const BoxCollider &box) {
  Math::Vector3 d = point - (box.GetTransform().GetWorldPos() + box.center);
  Math::Vector3 pt = (box.GetTransform().GetWorldPos() + box.center);

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
}  // namespace Isetta
