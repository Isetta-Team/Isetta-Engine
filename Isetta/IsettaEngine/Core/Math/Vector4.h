// "Copyright [2018] Isetta"
#pragma once
#include "ISETTA_API.h"

namespace Isetta {
class Color;
}

namespace Isetta::Math {
class ISETTA_API Vector4 {
 public:
  // Constants
  static const Vector4 zero;
  static const Vector4 one;
  static const Vector4 right;
  static const Vector4 left;
  static const Vector4 up;
  static const Vector4 down;
  static const Vector4 forward;
  static const Vector4 back;
  static const int ELEMENT_COUNT = 4;

  union {
    struct {
      float x, y, z, w;
    };
    float xyzw[ELEMENT_COUNT];
  };

  // Construct by name

  /**
   * \brief Create an empty vector
   */
  Vector4() : x{0}, y{0}, z{0}, w{0} {}
  /**
   * \brief Create a vector of a specific number
   * \param value the values of the vector
   */
  explicit Vector4(float value) : x{value}, y{value}, z{value}, w{value} {}
  /**
   * \brief Create a vector of specific x, y and z
   * \param inX The x of vector
   * \param inY The y of vector
   * \param inZ The z of vector
   * \param inW The w of vector
   */
  Vector4(float inX, float inY, float inZ, float inW)
      : x{inX}, y{inY}, z{inZ}, w{inW} {}
  Vector4(const class Vector3& inVector, float inW);
  explicit Vector4(const Color& c);

  // Copy and move constructions

  Vector4(const Vector4& inVector)
      : x{inVector.x}, y{inVector.y}, z{inVector.z}, w{inVector.w} {}
  Vector4(Vector4&& inVector) noexcept
      : x{inVector.x}, y{inVector.y}, z{inVector.z}, w{inVector.w} {}
  inline Vector4& operator=(const Vector4& inVector) {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    w = inVector.w;
    return *this;
  }
  inline Vector4& operator=(Vector4&& inVector) noexcept {
    x = inVector.x;
    y = inVector.y;
    z = inVector.z;
    w = inVector.w;
    return *this;
  }

  ~Vector4() {}

  // Operators

  float operator[](int i) const;
  float& operator[](int i);
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
  inline Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
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
  inline friend Vector4 operator*(float scalar, Vector4 v) {
    return v * scalar;
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
  explicit operator Color();
  explicit operator class Vector3() const;

  // Functions

  void Set(const class Vector3& inXYZ, float inW);
  /**
   * \brief Will abandon component w
   */
  Vector3 GetVector3() const;
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
  Vector4 Normalized() const;
  /**
   * \brief Normalizes current vector
   */
  void Normalize() noexcept;

  // Static Functions

  /**
   * \brief Checks if two vectors are equal (within a tolerance)
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static bool FuzzyEqual(const Vector4& lhs, const Vector4& rhs);
  /**
   * \brief Returns the dot product of two vectors
   * \param lhs The left vector
   * \param rhs The right vector
   */
  static float Dot(const Vector4& lhs, const Vector4& rhs);
  /**
   * \brief Linearly interpolates between two vectors
   * \param start The starting vector
   * \param end The ending vector
   * \param time The time t
   */
  static Vector4 Lerp(const Vector4& start, const Vector4& end, float time);
  /**
   * \brief The distance between two endpoints of the two vectors
   * \param start The starting vector
   * \param end The ending vector
   */
  static float Distance(const Vector4& start, const Vector4& end);
  /**
   * \brief Projects a vector onto onNormal vector
   * \param inVector The in vector
   * \param onNormal The target normal vector
   */
  static Vector4 Project(const Vector4& inVector, const Vector4& onNormal);
  /**
   * \brief Multiplies two vectors component-wise
   * \param aVector The input vector
   * \param bVector The scalar vector
   */
  static Vector4 Scale(const Vector4& aVector, const Vector4& bVector);
  /**
   * \brief Spherically interpolates between two vectors
   * \param start The starting vector
   * \param end The ending vector
   * \param time The time t
   */
  static Vector4 Slerp(const Vector4& start, const Vector4& end, float time);
};
}  // namespace Isetta::Math
