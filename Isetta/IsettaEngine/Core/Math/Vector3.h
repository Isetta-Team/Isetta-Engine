/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "ISETTA_API.h"

namespace Isetta::Math {
class ISETTA_API Vector3 {
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
  static const int ELEMENT_COUNT = 3;

  union {
    struct {
      float x, y, z;
    };
    float xyz[ELEMENT_COUNT];
  };

  // Construction

  /**
   * \brief Create an empty vector
   */
  Vector3() : x{0}, y{0}, z{0} {}
  /**
   * \brief Create a vector of a specific number
   * \param value the values of the vector
   */
  explicit Vector3(float value) : x{value}, y{value}, z{value} {}
  /**
   * \brief Create a vector of specific x, y and z
   * \param inX The x of vector
   * \param inY The y of vector
   * \param inZ The z of vector
   */
  Vector3(float inX, float inY, float inZ) : x{inX}, y{inY}, z{inZ} {}
  Vector3(const class Vector2& inVector, float inZ);
  explicit Vector3(const class Vector3Int& inIntVector);

  // Copy and move constructions

  Vector3(const Vector3& inVector)
      : x{inVector.x}, y{inVector.y}, z{inVector.z} {}
  Vector3(Vector3&& inVector) noexcept
      : x{inVector.x}, y{inVector.y}, z{inVector.z} {}
  inline Vector3& operator=(const Vector3& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    return *this;
  }
  inline Vector3& operator=(Vector3&& inVector) noexcept {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    return *this;
  }

  // Conversions

  explicit operator class Vector2();

  ~Vector3() {}

  // Operators

  float operator[](int i) const;
  float& operator[](int i);
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
  inline Vector3 operator-() const { return Vector3(-x, -y, -z); }
  inline Vector3& operator-=(const Vector3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  inline Vector3 operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }
  inline friend Vector3 operator*(float scalar, Vector3 v) {
    return v * scalar;
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
  inline friend Vector3 operator/(float scalar, Vector3 v) {
    return Vector3(scalar / v.x, scalar / v.y, scalar / v.z);
  }
  inline Vector3& operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
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
  Vector3 Normalized() const;
  /**
   * \brief Normalizes current vector
   */
  void Normalize() noexcept;
  /**
   * \brief Convert the vector to a string
   */
  std::string ToString() const;
  float Max() const;
  float Min() const;

  // static functions

  /**
   * \brief Checks if two vectors are equal (within a tolerance)
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static bool FuzzyEqual(const Vector3& lhs, const Vector3& rhs);
  /**
   * \brief Returns the dot product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static float Dot(const Vector3& lhs, const Vector3& rhs);
  /**
   * \brief Returns the cross product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
  /**
   * \brief Linearly interpolates between two vectors
   * \param start The starting vector
   * \param end The ending vector
   * \param time The time t
   */
  static Vector3 Lerp(const Vector3& start, const Vector3& end, float time);
  /**
   * \brief The distance between two endpoints of the two vectors
   * \param start The starting vector
   * \param end The ending vector
   */
  static float Distance(const Vector3& start, const Vector3& end);
  /**
   * \brief Projects a vector onto onNormal vector
   * \param inVector The in vector
   * \param onNormal The target normal vector
   */
  static Vector3 Project(const Vector3& inVector, const Vector3& onNormal);
  /**
   * \brief Reflects a vector off the plane defined by a normal
   * \param inVector The input vector
   * \param inNormal The normal vector pointing into the surface
   */
  static Vector3 Reflect(const Vector3& inVector, const Vector3& inNormal);
  /**
   * \brief Multiplies two vectors component-wise
   * \param inVector The input vector
   * \param scalar The scalar vector
   */
  static Vector3 Scale(const Vector3& a, const Vector3& b);
  static Vector3 ReverseScale(const Vector3& a, const Vector3& b);
  /**
   * \brief Spherically interpolates between two vectors
   * \param start The starting vector
   * \param end The ending vector
   * \param time The time t
   */
  static Vector3 Slerp(const Vector3& start, const Vector3& end, float time);
  /**
   * \brief Create a vector from the string
   * \param str Reference string
   */
  static Vector3 FromString(const std::string_view str);
  static float AngleDeg(const Vector3& a, const Vector3& b);
  static float AngleRad(const Vector3& a, const Vector3& b);
};
}  // namespace Isetta::Math
