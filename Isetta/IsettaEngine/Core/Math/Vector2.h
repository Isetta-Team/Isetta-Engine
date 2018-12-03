/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "ISETTA_API.h"

namespace Isetta::Math {

class ISETTA_API Vector2 {
 public:
  // Constants
  static const Vector2 zero;
  static const Vector2 one;
  static const Vector2 up;
  static const Vector2 right;
  static const Vector2 down;
  static const Vector2 left;
  static const int ELEMENT_COUNT = 2;

  union {
    struct {
      float x, y;
    };
    float xy[ELEMENT_COUNT];
  };

  // Construct by name

  /**
   * \brief Create an empty vector
   */
  Vector2() : x{0}, y{0} {}
  /**
   * \brief Create a vector of a specific number
   * \param value the values of the vector
   */
  explicit Vector2(float value) : x{value}, y{value} {}
  /**
   * \brief Create a vector of specific x and y
   * \param inX The x of vector
   * \param inY The y of vector
   */
  Vector2(float inX, float inY);

  // Copy and move constructions

  Vector2(const Vector2& inVector) : x{inVector.x}, y{inVector.y} {}
  Vector2(Vector2&& inVector) noexcept : x{inVector.x}, y{inVector.y} {}
  Vector2& operator=(const Vector2& inVector) {
    x = inVector.x;
    y = inVector.y;
    return *this;
  }
  Vector2& operator=(Vector2&& inVector) noexcept {
    x = inVector.x;
    y = inVector.y;
    return *this;
  }

  // Conversions

  explicit Vector2(const class Vector2Int& inIntVector);

  ~Vector2() {}

  // Operators

  float operator[](int i) const;
  inline bool operator==(const Vector2& rhs) const {
    return x == rhs.x && y == rhs.y;
  }
  inline bool operator!=(const Vector2& rhs) const { return !(*this == rhs); }

  inline Vector2 operator+(const Vector2& rhs) const {
    return Vector2(x + rhs.x, y + rhs.y);
  }
  inline Vector2& operator+=(const Vector2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  inline Vector2 operator-(const Vector2& rhs) const {
    return Vector2(x - rhs.x, y - rhs.y);
  }
  inline friend Vector2 operator-(float, Vector2 rhs) {
    return Vector2(-rhs.x, -rhs.y);
  }
  inline Vector2& operator-=(const Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  inline Vector2 operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
  }
  inline friend Vector2 operator*(float scalar, Vector2 in) {
    return in * scalar;
  }
  inline Vector2& operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  inline Vector2 operator/(float scalar) const {
    return Vector2(x / scalar, y / scalar);
  }
  inline Vector2& operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  // functions

  /**
   * \brief Returns the length of the vector
   */
  float Magnitude() const;
  /**
   * \brief Returns the square of the length of the vector
   */
  float SqrMagnitude() const;
  /**
   * \brief Returns a normalized vector of this vector
   */
  Vector2 Normalized() const;
  /**
   * \brief Normalizes current vector
   */
  void Normalize() noexcept;
  /**
   * \brief Return the string from the vector2
   */
  std::string ToString() const;

  // static functions

  /**
   * \brief Checks if two vectors are equal (within a tolerance)
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static bool Equals(const Vector2& lhs, const Vector2& rhs);
  /**
   * \brief Returns the dot product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static float Dot(const Vector2& lhs, const Vector2& rhs);
  /**
   * \brief Returns the cross product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static float Cross(const Vector2& lhs, const Vector2& rhs);
  /**
   * \brief Linearly interpolates between two vectors
   * \param start The starting vector
   * \param end The ending vector
   * \param time The time t
   */
  static Vector2 Lerp(const Vector2& start, const Vector2& end, float time);
  /**
   * \brief The distance between two endpoints of the two vectors
   * \param start The starting vector
   * \param end The ending vector
   */
  static float Distance(const Vector2& start, const Vector2& end);
  /**
   * \brief Projects a vector onto onNormal vector
   * \param inVector The in vector
   * \param onNormal The target normal vector
   */
  static Vector2 Project(const Vector2& inVector, const Vector2& onNormal);
  /**
   * \brief Reflects a vector off the plane defined by a normal
   * \param inVector The input vector
   * \param inNormal The normal vector pointing into the surface
   */
  static Vector2 Reflect(const Vector2& inVector, const Vector2& inNormal);
  /**
   * \brief Multiplies two vectors component-wise
   * \param inVector The input vector
   * \param scalar The scalar vector
   */
  static Vector2 Scale(const Vector2& inVector, const Vector2& scalar);
  /**
   * \brief Spherically interpolates between two vectors
   * \param start The starting vector
   * \param end The ending vector
   * \param time The time t
   */
  static Vector2 Slerp(const Vector2& start, const Vector2& end, float time);
  /**
   * \brief Checks if two vectors are equal (within a tolerance)
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static bool FuzzyEqual(const Vector2& lhs, const Vector2& rhs);
};  // namespace Isetta::Math
}  // namespace Isetta::Math
