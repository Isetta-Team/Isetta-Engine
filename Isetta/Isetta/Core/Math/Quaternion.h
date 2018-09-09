/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta::Math {
class Quaternion {
 public:
  float w, x, y, z;

  Quaternion() : w{0.f}, x{0.f}, y{0.f}, z{0.f} {}
  Quaternion(float inX, float inY, float inZ, float inW)
      : w{inW}, x{inX}, y{inY}, z{inZ} {}
  Quaternion(float eulerX, float eulerY, float eulerZ);
  Quaternion(class Vector3 vector, float scalar);

  Quaternion(const Quaternion& inQuaternion);
  Quaternion(Quaternion&& inQuaternion);
  Quaternion& operator=(const Quaternion& inQuaternion);
  Quaternion& operator=(Quaternion&& inQuaternion);

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

  class Vector3 GetEulerAngles() const;
  void SetFromToRotation(const class Vector3& fromDirection,
                         const class Vector3& toDirection);
  void SetLookRotation(const class Vector3& forwardDirection,
                       const class Vector3& upDirection);

  static float Angle(const Quaternion& aQuaternion,
                     const Quaternion& bQuaternion);
  static float Dot(const Quaternion& aQuaternion,
                   const Quaternion& bQuaternion);
  static Quaternion Inverse(const Quaternion& quaternion);
  static Quaternion Lerp(const Quaternion& aQuaternion,
                         const Quaternion& bQuaternion, float t);
  static Quaternion Normalize(const Quaternion& quaternion);
  static Quaternion Slerp(const Quaternion& aQuaternion,
                          const Quaternion& bQuaternion, float t);
};

Quaternion operator"" _i(long double inX);
Quaternion operator"" _j(long double inY);
Quaternion operator"" _k(long double inZ);
Quaternion operator"" _w(long double inW);
}  // namespace Isetta::Math
