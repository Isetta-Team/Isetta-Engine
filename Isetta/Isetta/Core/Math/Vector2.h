/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta::Math {

class Vector2 {
 public:
  // Constants
  static const Vector2 zero;
  static const Vector2 one;
  static const Vector2 up;
  static const Vector2 right;
  static const Vector2 down;
  static const Vector2 left;

  float x, y;

  // Construct by name

  Vector2() : x{0}, y{0} {}
  explicit Vector2(float value) : x{value}, y{value} {}
  Vector2(float inX, float inY) : x{inX}, y{inY} {}

  // Copy and move constructions

  Vector2(const Vector2& inVector) : x{inVector.x}, y{inVector.y} {}
  Vector2(Vector2&& inVector) : x{inVector.x}, y{inVector.y} {}
  inline Vector2& operator=(const Vector2& inVector) {
    x = inVector.x;
    y = inVector.y;
    return *this;
  }
  inline Vector2& operator=(Vector2&& inVector) {
    x = inVector.x;
    y = inVector.y;
    return *this;
  }

  // Conversions

  explicit Vector2(const class Vector3Int& inIntVector);
  // Vector3(const Vector2& inVector, float inZ);

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
  inline Vector2& operator-=(const Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }
  inline Vector2 operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
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

  // Returns the length of the vector
  float Magnitude() const;
  // Returns the square of the lenght of the vector
  float SqrMagnitude() const;
  // Returns a normalized vector of this vector
  Vector2 Normalized() const;
  // Normalizes current vector
  void Normalize() noexcept;

  // static functions

  // Checks if two vectors are equal (within a tolerence)
  static bool Equals(const Vector2& lhs, const Vector2& rhs);
  // Returns the dot product of two vectors
  static float Dot(const Vector2& lhs, const Vector2& rhs);
  // Returns the cross product of two vectors
  static float Cross(const Vector2& lhs, const Vector2& rhs);
  // Linearly interpolates between two vectors
  static Vector2 Lerp(const Vector2& start, const Vector2& end, float time);
  // Returns the distance between two endpoints of the vectors
  static float Distance(const Vector2& start, const Vector2& end);
  // Projects a vector onto onNormal vector
  static Vector2 Project(const Vector2& inVector, const Vector2& onNormal);
  // Reflects a vector off the plane defined by a normal
  static Vector2 Reflect(const Vector2& inVector, const Vector2& inNormal);
  // Multiplies two vectors component-wise
  static Vector2 Scale(const Vector2& inVector, const Vector2& scalar);
  // Spherically interpolates between two vectors
  static Vector2 Slerp(const Vector2& start, const Vector2& end, float time);
};
}  // namespace Isetta::Math
