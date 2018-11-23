/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "ISETTA_API.h"

namespace Isetta::Math {
class ISETTA_API Vector2Int {
 public:
  // Constants
  static const Vector2Int zero;
  static const Vector2Int one;
  static const Vector2Int up;
  static const Vector2Int right;
  static const Vector2Int down;
  static const Vector2Int left;
  static const int ELEMENT_COUNT = 2;

  union {
    struct {
      int x, y;
    };
    int xy[ELEMENT_COUNT];
  };

  // Construct by name

  /**
   * \brief Create an empty vector
   */
  Vector2Int() : x{0}, y{0} {}
  /**
   * \brief Create a vector of a specific number
   * \param value the values of the vector
   */
  explicit Vector2Int(int value) : x{value}, y{value} {}
  /**
   * \brief Create a vector of specific x and y
   * \param inX The x of vector
   * \param inY The y of vector
   */
  Vector2Int(int inX, int inY) : x{inX}, y{inY} {}

  // Copy and move constructions

  Vector2Int(const Vector2Int& inVector) : x{inVector.x}, y{inVector.y} {}
  Vector2Int(Vector2Int&& inVector) : x{inVector.x}, y{inVector.y} {}
  inline Vector2Int& operator=(const Vector2Int& inVector) {
    x = inVector.x;
    y = inVector.y;
    return *this;
  }
  inline Vector2Int& operator=(Vector2Int&& inVector) {
    x = inVector.x;
    y = inVector.y;
    return *this;
  }

  // Conversions

  explicit Vector2Int(const class Vector2& inVector);
  // Vector2(const Vector2& inVector, float inZ);

  ~Vector2Int() {}

  // Operators

  int operator[](int i) const;
  inline bool operator==(const Vector2Int& rhs) const {
    return x == rhs.x && y == rhs.y;
  }
  inline bool operator!=(const Vector2Int& rhs) const {
    return !(*this == rhs);
  }

  inline Vector2Int operator+(const Vector2Int& rhs) const {
    return Vector2Int(x + rhs.x, y + rhs.y);
  }
  inline Vector2Int& operator+=(const Vector2Int& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  inline Vector2Int operator-(const Vector2Int& rhs) const {
    return Vector2Int(x - rhs.x, y - rhs.y);
  }
  inline Vector2Int& operator-=(const Vector2Int& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  inline Vector2Int operator*(int scalar) const {
    return Vector2Int(x * scalar, y * scalar);
  }
  inline friend Vector2Int operator*(int scalar, Vector2Int v) {
    return v * scalar;
  }
  inline Vector2Int& operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  inline Vector2Int operator/(int scalar) const {
    return Vector2Int(x / scalar, y / scalar);
  }
  inline Vector2Int& operator/=(int scalar) {
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
  int SqrMagnitude() const;
  /**
   * \brief Returns a normalized vector of this vector
   */
  Vector2 Normalized() const;

  // static functions

  /**
   * \brief Returns the dot product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static int Dot(const Vector2Int& lhs, const Vector2Int& rhs);
  /**
   * \brief Returns the cross product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static int Cross(const Vector2Int& lhs, const Vector2Int& rhs);
  /**
   * \brief The distance between two endpoints of the two vectors
   * \param start The starting vector
   * \param end The ending vector
   */
  static float Distance(const Vector2Int& start, const Vector2Int& end);
  /**
   * \brief Multiplies two vectors component-wise
   * \param inVector The input vector
   * \param scalar The scalar vector
   */
  static Vector2Int Scale(const Vector2Int& inVector, const Vector2Int& scalar);
};
}  // namespace Isetta::Math
