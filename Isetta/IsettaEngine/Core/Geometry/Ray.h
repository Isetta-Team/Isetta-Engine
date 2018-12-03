/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector3.h"

namespace Isetta {
/**
 * @brief Line is geometry has a non-normalized direction
 *
 */
class Line {
 protected:
  Math::Vector3 origin, direction;

 public:
  /**
   * @brief Construct a new Line object
   *
   * @param origin a point along the line
   * @param direction of the line
   */
  Line(const Math::Vector3& origin, const Math::Vector3& direction)
      : origin{origin}, direction{direction} {}

  /**
   * @brief Get the Origin
   *
   * @return Math::Vector3
   */
  inline Math::Vector3 GetOrigin() const { return origin; }
  /**
   * @brief Get the Direction of line (non-normalized)
   *
   * @return Math::Vector3
   */
  inline Math::Vector3 GetDirection() const { return direction; }
  /**
   * @brief Set the Origin (same as setting a point)
   *
   * @param o
   */
  inline void SetOrigin(Math::Vector3 o) { origin = o; }
  /**
   * @brief Set the Direction (non-normalized)
   *
   * @param d direction
   */
  inline virtual void SetDirection(Math::Vector3 d) { direction = d; }
  /**
   * @brief Get the Point along line
   *
   * @param distance from the origin of that point
   * @return Math::Vector3
   */
  inline Math::Vector3 GetPoint(float distance) const {
    return origin + distance * direction;
  }
};

class Ray : public Line {
 public:
  /**
   * @brief Construct a new Ray object
   *
   * @param origin start point of the ray
   * @param direction normalized along line
   */
  Ray(const Math::Vector3& origin, const Math::Vector3& direction)
      : Line(origin, direction.Normalized()) {}

  /**
   * @brief Set the Direction (normalized)
   *
   * @param d direction
   */
  inline void SetDirection(Math::Vector3 d) override {
    direction = d.Normalized();
  }
};
}  // namespace Isetta
