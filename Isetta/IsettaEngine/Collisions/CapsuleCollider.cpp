/*
 * Copyright (c) 2018 Isetta
 */
#include "Collisions/CapsuleCollider.h"
#include "Collisions/BoxCollider.h"
#include "Collisions/CollisionsModule.h"
#include "Collisions/RaycastHit.h"
#include "Collisions/SphereCollider.h"

#include "Core/Debug/DebugDraw.h"
#include "Core/Geometry/Ray.h"
#include "Core/Math/Matrix3.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Vector4.h"
#include "Scene/Transform.h"

namespace Isetta {
#if _EDITOR
void CapsuleCollider::Update() {
  Math::Matrix4 scale;
  Math::Matrix4 rotation;
  GetWorldCapsule(&rotation, &scale);
  DebugDraw::WireCapsule(
      Math::Matrix4::Translate(GetWorldCenter()) * rotation *
          scale,
      radius, height, debugColor);

  Math::Vector3 dir =
      0.5f * static_cast<Math::Vector3>(
                 (rotation * scale *
                  Math::Matrix4::Scale(Math::Vector3{height - 2 * radius}) *
                  Math::Vector4{0, 1, 0, 0}));
  Math::Vector3 P0 = GetWorldCenter() - dir;
  Math::Vector3 P1 = GetWorldCenter() + dir;
  DebugDraw::Line(P0, P1, Color::blue);
}
#endif

bool CapsuleCollider::RaycastSphere(const Math::Vector3& center, float radius,
                                    const Ray& ray, RaycastHit* const hitInfo,
                                    float maxDistance) {
  Math::Vector3 to = ray.GetOrigin() - center;
  float b = Math::Vector3::Dot(to, ray.GetDirection());
  float c = Math::Vector3::Dot(to, to) - radius * radius;
  if (c > 0.0f && b > 0.0f) {
    return false;
  }
  float discrim = b * b - c;
  if (discrim < 0.0f) {
    return false;
  }
  discrim = Math::Util::Sqrt(discrim);
  float t = -b - discrim;
  if (t < 0.f) t = -b + discrim;
  if (t > maxDistance) return false;
  Math::Vector3 pt = ray.GetPoint(t);
  RaycastHitCtor(hitInfo, t, pt, pt - center);
  return true;
}
float CapsuleCollider::GetWorldCapsule(Math::Matrix4* rotation,
                                       Math::Matrix4* scale) const {
  Math::Matrix4& rot = *rotation;
  rot = (Math::Matrix4)transform->GetWorldRot();
  float max;
  switch (direction) {
    case Direction::X_AXIS:
      rot *= Math::Matrix4::zRot90;
      // rot = rot * Math::Matrix4::zRot90;
      max = Math::Util::Max(transform->GetWorldScale().y,
                            transform->GetWorldScale().z);
      *scale = Math::Matrix4::Scale(
          Math::Vector3{transform->GetWorldScale().x, max, max});
      break;
    case Direction::Y_AXIS:
      max = Math::Util::Max(transform->GetWorldScale().x,
                            transform->GetWorldScale().z);
      *scale = Math::Matrix4::Scale(
          Math::Vector3{max, transform->GetWorldScale().y, max});
      break;
    case Direction::Z_AXIS:
      *rotation *= Math::Matrix4::xRot90;
      max = Math::Util::Max(transform->GetWorldScale().x,
                            transform->GetWorldScale().y);
      *scale = Math::Matrix4::Scale(
          Math::Vector3{max, max, transform->GetWorldScale().z});
      break;
  }
  return max;
}
bool CapsuleCollider::Raycast(const Ray& ray, RaycastHit* const hitInfo,
                              float maxDistance) {
  maxDistance = maxDistance <= 0 ? INFINITY : maxDistance;
  Math::Matrix4 rot, scale;
  float radiusScale = GetWorldCapsule(&rot, &scale);
  Math::Vector3 dir = (Math::Vector3)(
      rot * scale * Math::Matrix4::Scale(Math::Vector3{height - 2 * radius}) *
      Math::Vector4{0, 1, 0, 0});
  Math::Vector3 p0 = GetWorldCenter() - dir;
  Math::Vector3 p1 = GetWorldCenter() + dir;

  Math::Vector3 to = p1 - p0;
  Math::Vector3 o = ray.GetOrigin() - p0;

  float toDir = Math::Vector3::Dot(to, ray.GetDirection());
  float toO = Math::Vector3::Dot(to, o);
  float toDot = Math::Vector3::Dot(to, to);

  float m = toDir / toDot;
  float n = toO / toDot;

  Math::Vector3 q = ray.GetDirection() - (to * m);
  Math::Vector3 r = o - (to * n);

  float a = Math::Vector3::Dot(q, q);
  float b = 2.0f * Math::Vector3::Dot(q, r);
  float c = Math::Vector3::Dot(r, r) - Math::Util::Square(radius * radiusScale);

  if (a == 0.0f) {
    RaycastHit aHit, bHit;
    if (!RaycastSphere(p0, radius * radiusScale, ray, &aHit, maxDistance) ||
        !RaycastSphere(p0, radius * radiusScale, ray, &aHit, maxDistance))
      return false;
    if (aHit.GetDistance() < bHit.GetDistance())
      *hitInfo = std::move(aHit);
    else
      *hitInfo = std::move(bHit);
    return true;
  }

  float discrim = b * b - 4.f * a * c;
  if (discrim < 0.f) return false;
  float sqrtDiscrim = Math::Util::Sqrt(discrim);
  float denom = 0.5f / a;
  float tmin = -(b + sqrtDiscrim) * denom;
  float tmax = (-b + sqrtDiscrim) * denom;
  if (tmin > tmax) std::swap(tmin, tmax);
  if (tmin > maxDistance) return false;

  float tkMin = tmin * m + n;
  if (tkMin < 0.f) {
    return RaycastSphere(p0, radius * radiusScale, ray, hitInfo, maxDistance);
  } else if (tkMin > 1.f) {
    return RaycastSphere(p1, radius * radiusScale, ray, hitInfo, maxDistance);
  } else {
    Math::Vector3 pt = ray.GetPoint(tmin);
    RaycastHitCtor(hitInfo, tmin, pt, pt - (p0 + to * tkMin));
    // DebugDraw::Point(pt, Color::cyan, 10, 10);
    // DebugDraw::Point(p0 + to * tkMin, Color::cyan, 10, 10);
    return true;
  }

  return false;
}

AABB CapsuleCollider::GetFatAABB() {
  return AABB{GetWorldCenter(), 2.f *
                                    (GetWorldHeight() * transform->GetUp() +
                                     radius * Math::Vector3::one) *
                                    (1 + fatFactor)};
}
AABB CapsuleCollider::GetAABB() {
  // TODO(Yidi) + TODO(Jacob) not a tight AABB
  return AABB{GetWorldCenter(), 2.f * (GetWorldHeight() * transform->GetUp() +
                                       radius * Math::Vector3::one)};
}

INTERSECTION_TEST(CapsuleCollider)
}  // namespace Isetta
