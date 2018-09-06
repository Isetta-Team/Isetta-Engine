/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta::Math {

class Vector3Int {
 public:
  // Constants
  static const Vector3Int zero;
  static const Vector3Int one;
  static const Vector3Int up;
  static const Vector3Int right;
  static const Vector3Int forward;
  static const Vector3Int down;
  static const Vector3Int left;
  static const Vector3Int back;

  int x, y, z;

  // Construct by name

  Vector3Int() : x{0}, y{0}, z{0} {}
  explicit Vector3Int(int value) : x{value}, y{value}, z{value} {}
  Vector3Int(int inX, int inY, int inZ) : x{inX}, y{inY}, z{inZ} {}

  // Copy and move constructions

  Vector3Int(const Vector3Int& inVector)
      : x{inVector.x}, y{inVector.y}, z{inVector.z} {}
  Vector3Int(Vector3Int&& inVector)
      : x{inVector.x}, y{inVector.y}, z{inVector.z} {}
  inline Vector3Int& operator=(const Vector3Int& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    return *this;
  }
  inline Vector3Int& operator=(Vector3Int&& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    return *this;
  }

  // Conversions

  explicit Vector3Int(const class Vector3& inIntVector);
  Vector3Int(const class Vector2Int& inVector, int inZ);

  ~Vector3Int() {}

  // Operators

  int operator[](int i) const;
  inline bool operator==(const Vector3Int& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }
  inline bool operator!=(const Vector3Int& rhs) const {
    return !(*this == rhs);
  }

  inline Vector3Int operator+(const Vector3Int& rhs) const {
    return Vector3Int(x + rhs.x, y + rhs.y, z + rhs.z);
  }
  inline Vector3Int& operator+=(const Vector3Int& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  inline Vector3Int operator-(const Vector3Int& rhs) const {
    return Vector3Int(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  inline Vector3Int& operator-=(const Vector3Int& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  inline Vector3Int operator*(int scalar) const {
    return Vector3Int(x * scalar, y * scalar, z * scalar);
  }
  inline Vector3Int& operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }
  inline Vector3Int operator/(int scalar) const {
    return Vector3Int(x / scalar, y / scalar, z / scalar);
  }
  inline Vector3Int& operator/=(int scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  }

  // functions

  // Returns the length of the vector
  float Magnitude() const;
  // Returns the square of the lenght of the vector
  int SqrMagnitude() const;
  // Returns a normalized vector of this vector
  Vector3 Normalized() const;

  // static functions

  // Returns the dot product of two vectors
  static int Dot(const Vector3Int& lhs, const Vector3Int& rhs);
  // Returns the cross product of two vectors
  static Vector3Int Cross(const Vector3Int& lhs, const Vector3Int& rhs);
  // Returns the distance between two endpoints of the vectors
  static float Distance(const Vector3Int& start, const Vector3Int& end);
  // Multiplies two vectors component-wise
  static Vector3Int Scale(const Vector3Int& aVector, const Vector3Int& bVector);
};
}  // namespace Isetta::Math
