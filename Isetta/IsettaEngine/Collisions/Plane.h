/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector3.h"

namespace Isetta {
class Plane {
 private:
  Math::Vector3 normal;
  float distance;

 public:
  Plane(const Math::Vector3& normal, const Math::Vector3& point)
      : normal{normal.Normalized()}, distance{point.Magnitude()} {}
  Plane(const Math::Vector3& normal, float distance)
      : normal{normal.Normalized()}, distance{distance} {}
  Plane(const Math::Vector3& a, const Math::Vector3& b, const Math::Vector3& c);

  inline Math::Vector3 GetNormal() const { return normal; }
  inline float GetDistance() const { return distance; }
  inline void Flip() { normal *= -1; }

  void SetPoints(const Math::Vector3& a, const Math::Vector3& b,
                 const Math::Vector3& c);
  void SetNormalPosition(const Math::Vector3& inNormal,
                         const Math::Vector3& point);

  Math::Vector3 ClosestPointOnPlane(const Math::Vector3& point);
  float GetDistanceToPoint(const Math::Vector3& point);
  int GetSide(const Math::Vector3& point);
  bool SameSide(const Math::Vector3& pt0, const Math::Vector3& pt1);
  bool Raycast(const class Ray& ray, class RaycastHit* const hitInfo,
               float maxDistance = 0);
};
}  // namespace Isetta
