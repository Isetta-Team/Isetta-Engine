// "Copyright [2018] Isetta"
#pragma once
#ifndef ISETTA_ISETTA_MATH_VECTOR4_H_
#define ISETTA_ISETTA_MATH_VECTOR4_H_

namespace Isetta::Math {

class Vector4 {
 public:
  float x, y, z, w;

  // Construct by name

  Vector4() : x{0}, y{0}, z{0}, w{0} {}
  explicit Vector4(float value) : x{value}, y{value}, z{value}, w{value} {}
  Vector4(float inX, float inY, float inZ, float inW)
      : x{inX}, y{inY}, z{inZ}, w{inW} {}

  // Copy and move constructions

  Vector4(const Vector4& inVector)
      : x{inVector.x}, y{inVector.y}, z{inVector.z}, w{inVector.w} {}
  Vector4(Vector4&& inVector)
      : x{inVector.x}, y{inVector.y}, z{inVector.z}, w{inVector.w} {}
  inline Vector4& operator=(const Vector4& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    w = inVector.w;
    return *this;
  }
  inline Vector4& operator=(Vector4&& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    w = inVector.w;
    return *this;
  }

  ~Vector4() {}

  // Operators

  float operator[](int i) const;
  inline bool operator==(const Vector4& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
  }
  inline bool operator!=(const Vector4& rhs) const { return !(*this == rhs); }

  inline Vector4 operator+(const Vector4& rhs) const {
    return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
  }
  inline Vector4& operator+=(const Vector4& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
  }
  inline Vector4 operator-(const Vector4& rhs) const {
    return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
  }
  inline Vector4& operator-=(const Vector4& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
  }
  inline Vector4 operator*(float scalar) const {
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
  }
  inline Vector4& operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }
  inline Vector4 operator/(float scalar) const {
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
  }
  inline Vector4& operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
  }

  // Conversions

  // TODO: Fix
  // explicit Vector4(const class Color& c);

  // Functions

  // Returns the length of the vector
  float Magnitude() const;
  // Returns the square of the lenght of the vector
  float SqrMagnitude() const;
  // Returns a normalized vector of this vector
  Vector4 Normalized() const;
  // Normalizes current vector
  void Normalize() noexcept;

  // Static Functions

  // Checks if two vectors are equal (within a tolerence)
  static bool Equals(const Vector4& lhs, const Vector4& rhs);
  // Returns the dot product of two vectors
  static float Dot(const Vector4& lhs, const Vector4& rhs);
  // Linearly interpolates between two vectors
  static Vector4 Lerp(const Vector4& start, const Vector4& end, float time);
  // Returns the distance between two endpoints of the vectors
  static float Distance(const Vector4& start, const Vector4& end);
  // Projects a vector onto onNormal vector
  static Vector4 Project(const Vector4& inVector, const Vector4& onNormal);
  // Multiplies two vectors component-wise
  static Vector4 Scale(const Vector4& inVector, const Vector4& scalar);
  // Spherically interpolates between two vectors
  static Vector4 Slerp(const Vector4& start, const Vector4& end, float time);

  // Constants
  static const Vector4 zero;
  static const Vector4 one;
};
}  // namespace Isetta::Math

#endif  // ISETTA_ISETTA_MATH_Vector4_H_
