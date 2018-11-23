/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "ISETTA_API.h"

namespace Isetta::Math {
class ISETTA_API Vector3Int {
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
  static const int ELEMENT_COUNT = 3;

  union {
    struct {
      int x, y, z;
    };
    int xyz[ELEMENT_COUNT];
  };

  // Construct by name

  /**
   * \brief Create an empty vector
   */
  Vector3Int() : x{0}, y{0}, z{0} {}
  /**
   * \brief Create a vector of a specific number
   * \param value the values of the vector
   */
  explicit Vector3Int(int value) : x{value}, y{value}, z{value} {}
  /**
   * \brief Create a vector of specific x, y and z
   * \param inX The x of vector
   * \param inY The y of vector
   * \param inZ The z of vector
   */
  Vector3Int(int inX, int inY, int inZ) : x{inX}, y{inY}, z{inZ} {}
  Vector3Int(const class Vector2Int& inVector, float inZ);

  // Copy and move constructions

  Vector3Int(const Vector3Int& inVector)
      : x{inVector.x}, y{inVector.y}, z{inVector.z} {}
  Vector3Int(Vector3Int&& inVector) noexcept
      : x{inVector.x}, y{inVector.y}, z{inVector.z} {}
  inline Vector3Int& operator=(const Vector3Int& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    return *this;
  }
  inline Vector3Int& operator=(Vector3Int&& inVector) noexcept {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    return *this;
  }

  // Conversions

  explicit Vector3Int(const class Vector3& inVector);
  Vector3Int(const class Vector2Int& inVector, int inZ);

  explicit operator class Vector2Int();

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
  inline friend Vector3Int operator*(float scalar, Vector3Int v) {
    return v * scalar;
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
  Vector3 Normalized() const;

  // static functions

  /**
   * \brief Returns the dot product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static int Dot(const Vector3Int& lhs, const Vector3Int& rhs);
  /**
   * \brief Returns the cross product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static Vector3Int Cross(const Vector3Int& lhs, const Vector3Int& rhs);
  /**
   * \brief The distance between two endpoints of the two vectors
   * \param start The starting vector
   * \param end The ending vector
   */
  static float Distance(const Vector3Int& start, const Vector3Int& end);
  /**
   * \brief Multiplies two vectors component-wise
   * \param aVector The input vector
   * \param bVector The scalar vector
   */
  static Vector3Int Scale(const Vector3Int& aVector, const Vector3Int& bVector);
};
}  // namespace Isetta::Math
