/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Geometry/Plane.h"

#include "Collisions/RaycastHit.h"
#include "Core/Geometry/Ray.h"
#include "Core/Math/Util.h"

namespace Isetta {
Plane::Plane(const Math::Vector3& a, const Math::Vector3& b,
             const Math::Vector3& c) {
  normal = Math::Vector3::Cross(a, b);
  distance = Math::Vector3::Dot(normal, c);
  normal.Normalize();
}
void Plane::SetPoints(const Math::Vector3& a, const Math::Vector3& b,
                      const Math::Vector3& c) {
  normal = Math::Vector3::Cross(a, b);
  distance = Math::Vector3::Dot(normal, c);
  normal.Normalize();
}
void Plane::SetNormalPosition(const Math::Vector3& inNormal,
                              const Math::Vector3& point) {
  normal = inNormal.Normalized();
  distance = point.Magnitude();
}
Math::Vector3 Plane::ClosestPointOnPlane(const Math::Vector3& point) {
  float t = (Math::Vector3::Dot(normal, point) - distance) /
            Math::Vector3::Dot(normal, normal);
  return point - t * normal;
}
float Plane::GetDistanceToPoint(const Math::Vector3& point) {
  return (Math::Vector3::Dot(normal, point) - distance) /
         Math::Vector3::Dot(normal, normal);
}
int Plane::GetSide(const Math::Vector3& point) {
  Math::Vector3 to = distance * normal - point;
  return Math::Vector3::Dot(normal, to) > 0 ? -1 : 1;
}
bool Plane::SameSide(const Math::Vector3& pt0, const Math::Vector3& pt1) {
  Math::Vector3 pt = distance * normal;
  return Math::Vector3::Dot(pt - pt0, pt - pt1) >= 0;
}
bool Plane::Raycast(const Ray& ray, RaycastHit* const hitInfo,
                    float maxDistance) {
  float denom = Math::Vector3::Dot(normal, ray.GetDirection());
  if (Math::Util::Abs(denom) > Math::Util::EPSILON) {
    float t =
        Math::Vector3::Dot(normal * distance - ray.GetOrigin(), normal) / denom;
    if (t >= 0) {
      *hitInfo = std::move(RaycastHit{nullptr, t, ray.GetPoint(t), normal});
      return true;
    }
  }
  return false;
}
}  // namespace Isetta
