/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector3.h"

namespace Isetta {
class Line {
 protected:
  Math::Vector3 origin, direction;

 public:
  Line(const Math::Vector3& origin, const Math::Vector3& direction)
      : origin{origin}, direction{direction} {}

  inline Math::Vector3 GetOrigin() const { return origin; }
  inline Math::Vector3 GetDirection() const { return direction; }
  inline void SetOrigin(Math::Vector3 o) { origin = o; }
  inline virtual void SetDirection(Math::Vector3 d) { direction = d; }
  inline Math::Vector3 GetPoint(float distance) const {
    return origin + distance * direction;
  }
};

class Ray : public Line {
 public:
  Ray(const Math::Vector3& origin, const Math::Vector3& direction)
      : Line(origin, direction.Normalized()) {}

  inline void SetDirection(Math::Vector3 d) override {
    direction = d.Normalized();
  }
};

class RaycastHit {
 private:
  class Collider* collider;
  float distance{INFINITY};
  Math::Vector3 point, normal;

  RaycastHit(class Collider* collider, float distance, Math::Vector3 point,
             Math::Vector3 normal)
      : collider{collider}, distance{distance}, point{point}, normal{normal} {}

 public:
  inline class Collider* GetCollider() { return collider; }
  inline float GetDistance() { return distance; }
  inline Math::Vector3 GetPoint() { return point; }
  inline Math::Vector3 GetNormal() { return normal; }

  RaycastHit() = default;
  RaycastHit& operator=(RaycastHit&& inHit) noexcept {
    collider = inHit.collider;
    inHit.collider = nullptr;
    distance = inHit.distance;
    inHit.distance = INFINITY;
    point = inHit.point;
    normal = inHit.normal;
    return *this;
  }

  friend class Collider;
  friend class Plane;
  friend class AABB;
  friend class BVTree;
};
}  // namespace Isetta
