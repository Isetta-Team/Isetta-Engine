/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta::Math {
class Quaternion {
 public:
  float w, x, y, z;

  /**
   * \brief Create a zero rotation quaternion
   */
  Quaternion();

  /**
   * \brief Create a quaternion in w + xi + yj + zk form
   * \param inX X value of the quaternion
   * \param inY Y value of the quaternion
   * \param inZ Z value of the quaternion
   * \param inW W value of the quaternion
   */
  Quaternion(float inX, float inY, float inZ, float inW)
      : w{inW}, x{inX}, y{inY}, z{inZ} {}
  /**
   * \brief Create a quaternion from euler angles
   * \param eulerX Euler angle in degree around X axis
   * \param eulerY Euler angle in degree around Y axis
   * \param eulerZ Euler angle in degree around Z axis
   */
  Quaternion(float eulerX, float eulerY, float eulerZ);
  /**
   * \brief Create a quaterion from an axis and an angle
   * \param vector The axis
   * \param scalar The angle
   */
  Quaternion(class Vector3 vector, float scalar);

  Quaternion(const Quaternion& inQuaternion);
  Quaternion(Quaternion&& inQuaternion) noexcept;
  Quaternion& operator=(const Quaternion& inQuaternion);
  Quaternion& operator=(Quaternion&& inQuaternion) noexcept;

  ~Quaternion() {}

  bool operator==(const Quaternion& rhs) const;
  bool operator!=(const Quaternion& rhs) const;
  Quaternion operator+(const Quaternion& rhs) const;
  Quaternion& operator+=(const Quaternion& rhs);
  Quaternion operator-(const Quaternion& rhs) const;
  Quaternion& operator-=(const Quaternion& rhs);
  Quaternion operator*(const Quaternion& rhs) const;
  Quaternion& operator*=(const Quaternion& rhs);
  Quaternion operator*(float scalar) const;

  /**
   * \brief Get the euler angles of the quaternion
   */
  class Vector3 GetEulerAngles() const;
  /**
   * \brief Set the quaternion with a pair of from and to direction
   * \param fromDirection The starting from direction
   * \param toDirection The target to direction
   */
  void SetFromToRotation(const class Vector3& fromDirection,
                         const class Vector3& toDirection);
  /**
   * \brief Set the quaternion with a pair of forward and up direction
   * \param forwardDirection The forward vector
   * \param upDirection The up vector
   */
  void SetLookRotation(const class Vector3& forwardDirection,
                       const class Vector3& upDirection);

  /**
   * \brief Return the angle between two quaternions
   * \param aQuaternion Quaternion a
   * \param bQuaternion Quaternion b
   */
  static float Angle(const Quaternion& aQuaternion,
                     const Quaternion& bQuaternion);
  /**
   * \brief Return the dot product of two quaternion
   * \param aQuaternion Quaternion a
   * \param bQuaternion Quaternion
   */
  static float Dot(const Quaternion& aQuaternion,
                   const Quaternion& bQuaternion);
  /**
   * \brief Return the inverse quaternion
   * \param quaternion The quaternion
   */
  static Quaternion Inverse(const Quaternion& quaternion);
  /**
   * \brief Lerp between two quaternions by time t
   * \param aQuaternion The starting quaternion
   * \param bQuaternion The ending quaternion
   * \param t Time t
   */
  static Quaternion Lerp(const Quaternion& aQuaternion,
                         const Quaternion& bQuaternion, float t);
  /**
   * \brief Normalize the quaternion
   * \param quaternion The quaternion
   */
  static Quaternion Normalize(const Quaternion& quaternion);
  /**
   * \brief Sphere lerp between two quaternions by t
   * \param aQuaternion The starting quaternion
   * \param bQuaternion The ending quaternion
   * \param t Time t
   */
  static Quaternion Slerp(const Quaternion& aQuaternion,
                          const Quaternion& bQuaternion, float t);
};

Quaternion operator"" _i(long double inX);
Quaternion operator"" _j(long double inY);
Quaternion operator"" _k(long double inZ);
Quaternion operator"" _w(long double inW);
}  // namespace Isetta::Math
