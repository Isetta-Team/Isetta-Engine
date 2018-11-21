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
}  // namespace Isetta
