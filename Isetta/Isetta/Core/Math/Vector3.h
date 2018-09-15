/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>

namespace Isetta::Math {

class Vector3 {
 public:
  // Constants
  static const Vector3 zero;
  static const Vector3 one;
  static const Vector3 up;
  static const Vector3 right;
  static const Vector3 forward;
  static const Vector3 down;
  static const Vector3 left;
  static const Vector3 back;

  float x, y, z;

  // Construction

  Vector3() : x{0}, y{0}, z{0} {}
  explicit Vector3(float value) : x{value}, y{value}, z{value} {}
  Vector3(float inX, float inY, float inZ) : x{inX}, y{inY}, z{inZ} {}

  // Copy and move constructions

  Vector3(const Vector3& inVector)
      : x{inVector.x}, y{inVector.y}, z{inVector.z} {}
  Vector3(Vector3&& inVector) noexcept : x{inVector.x}, y{inVector.y}, z{inVector.z} {}
  inline Vector3& operator=(const Vector3& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    return *this;
  }
  inline Vector3& operator=(Vector3&& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    return *this;
  }

  // Conversions

  explicit Vector3(const class Vector3Int& inIntVector);
  Vector3(const class Vector2& inVector, float inZ);

  ~Vector3() {}

  // Operators

  float operator[](int i) const;
  inline bool operator==(const Vector3& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }
  inline bool operator!=(const Vector3& rhs) const { return !(*this == rhs); }

  inline Vector3 operator+(const Vector3& rhs) const {
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
  }
  inline Vector3& operator+=(const Vector3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  inline Vector3 operator-(const Vector3& rhs) const {
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  inline Vector3& operator-=(const Vector3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  inline Vector3 operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }
  inline Vector3& operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }
  inline Vector3 operator/(float scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
  }
  inline Vector3& operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  // functions

  // Returns the length of the vector
  float Magnitude() const;
  // Returns the square of the lenght of the vector
  float SqrMagnitude() const;
  // Returns a normalized vector of this vector
  Vector3 Normalized() const;
  // Normalizes current vector
  void Normalize() noexcept;
  std::string ToString() const;

  // static functions

  // Checks if two vectors are equal (within a tolerence)
  static bool Equals(const Vector3& lhs, const Vector3& rhs);
  // Returns the dot product of two vectors
  static float Dot(const Vector3& lhs, const Vector3& rhs);
  // Returns the cross product of two vectors
  static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
  // Linearly interpolates between two vectors
  static Vector3 Lerp(const Vector3& start, const Vector3& end, float time);
  // Returns the distance between two endpoints of the vectors
  static float Distance(const Vector3& start, const Vector3& end);
  // Projects a vector onto onNormal vector
  static Vector3 Project(const Vector3& inVector, const Vector3& onNormal);
  // Reflects a vector off the plane defined by a normal
  static Vector3 Reflect(const Vector3& inVector, const Vector3& inNormal);
  // Multiplies two vectors component-wise
  static Vector3 Scale(const Vector3& inVector, const Vector3& scalar);
  // Spherically interpolates between two vectors
  static Vector3 Slerp(const Vector3& start, const Vector3& end, float time);
  static Vector3 FromString(std::string str);
};
}  // namespace Isetta::Math
