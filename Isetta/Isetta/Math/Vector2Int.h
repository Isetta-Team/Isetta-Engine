// "Copyright [2018] Isetta"
#pragma once
#ifndef ISETTA_ISETTA_MATH_VECTOR2INT_H_
#define ISETTA_ISETTA_MATH_VECTOR2INT_H_

namespace Isetta::Math {

class Vector2Int {
 public:
  int x, y;

  // Construct by name

  Vector2Int() : x{0}, y{0} {}
  explicit Vector2Int(int value) : x{value}, y{value} {}
  Vector2Int(int inX, int inY) : x{inX}, y{inY} {}

  // Copy and move constructions

  Vector2Int(const Vector2Int& inVector)
      : x{inVector.x}, y{inVector.y} {}
  Vector2Int(Vector2Int&& inVector)
      : x{inVector.x}, y{inVector.y} {}
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

  explicit Vector2Int(const class Vector2& inIntVector);
  // Vector2(const Vector2& inVector, float inZ);

  ~Vector2Int() {}

  // Operators

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

  // Returns the length of the vector
  float Magnitude() const;
  // Returns the square of the lenght of the vector
  int SqrMagnitude() const;
  // Returns a normalized vector of this vector
  Vector2 Normalized() const;

  // static functions

  // Returns the dot product of two vectors
  static int Dot(const Vector2Int& lhs, const Vector2Int& rhs);
  // Returns the cross product of two vectors
  static int Cross(const Vector2Int& lhs, const Vector2Int& rhs);
  // Returns the distance between two endpoints of the vectors
  static float Distance(const Vector2Int& start, const Vector2Int& end);
  // Multiplies two vectors component-wise
  static Vector2Int Scale(const Vector2Int& inVector, const Vector2Int& scalar);

  // Constants
  static const Vector2Int zero;
  static const Vector2Int one;
  static const Vector2Int up;
  static const Vector2Int right;
  static const Vector2Int down;
  static const Vector2Int left;
};
}  // namespace Math

#endif  // ISETTA_ISETTA_MATH_Vector2_H_
#pragma once
