/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Quaternion.h"

#include <sstream>
#include "Core/Math/Util.h"
#include "Core/Math/Vector3.h"
#include "Matrix3.h"

namespace Isetta::Math {
Quaternion::Quaternion() : w{0.f}, x{0.f}, y{0.f}, z{0.f} {}

Quaternion::Quaternion(float eulerX, float eulerY, float eulerZ) {
  Quaternion roll(Util::Sin(eulerX * 0.5f), 0, 0, Util::Cos(eulerX * 0.5f));
  Quaternion pitch(0, Util::Sin(eulerY * 0.5f), 0, Util::Cos(eulerY * 0.5f));
  Quaternion yaw(0, 0, Util::Sin(eulerZ * 0.5f), Util::Cos(eulerZ * 0.5f));

  // Order: y * x * z
  *this = (pitch * roll * yaw).Normalized();
}

Quaternion Quaternion::FromEulerAngles(const Vector3& eulerAngles) {
  return Quaternion{eulerAngles.x, eulerAngles.y, eulerAngles.z};
}

Quaternion Quaternion::FromEulerAngles(const float eulerX, const float eulerY,
                                       const float eulerZ) {
  return Quaternion{eulerX, eulerY, eulerZ};
}

Quaternion::Quaternion(const Vector3 vector, const float scalar)
    : w{scalar}, x{vector.x}, y{vector.y}, z{vector.z} {
  Normalize();
}

Quaternion Quaternion::FromAngleAxis(const Vector3& axis, const float angle) {
  return Quaternion{axis, angle};
}

Quaternion Quaternion::FromLookRotation(const Vector3& forwardDirection,
                                        const Vector3& upDirection) {
  Quaternion ret{};
  ret.SetLookRotation(forwardDirection, upDirection);
  return ret;
}

Quaternion::Quaternion(const Quaternion& inQuaternion)
    : w{inQuaternion.w},
      x{inQuaternion.x},
      y{inQuaternion.y},
      z{inQuaternion.z} {}

Quaternion::Quaternion(Quaternion&& inQuaternion) noexcept
    : w{inQuaternion.w},
      x{inQuaternion.x},
      y{inQuaternion.y},
      z{inQuaternion.z} {}

Quaternion& Quaternion::operator=(const Quaternion& inQuaternion) {
  w = inQuaternion.w;
  x = inQuaternion.x;
  y = inQuaternion.y;
  z = inQuaternion.z;

  return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& inQuaternion) noexcept {
  w = inQuaternion.w;
  x = inQuaternion.x;
  y = inQuaternion.y;
  z = inQuaternion.z;

  return *this;
}

bool Quaternion::operator==(const Quaternion& rhs) const {
  return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Quaternion::operator!=(const Quaternion& rhs) const {
  return !(*this == rhs);
}

Quaternion Quaternion::operator+(const Quaternion& rhs) const {
  return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
}

Quaternion& Quaternion::operator+=(const Quaternion& rhs) {
  w += rhs.w;
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}

Quaternion Quaternion::operator-(const Quaternion& rhs) const {
  return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
}

Quaternion& Quaternion::operator-=(const Quaternion& rhs) {
  w -= rhs.w;
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const {
  return Quaternion(y * rhs.z - z * rhs.y + rhs.x * w + x * rhs.w,
                    z * rhs.x - x * rhs.z + rhs.y * w + y * rhs.w,
                    x * rhs.y - y * rhs.x + rhs.z * w + z * rhs.w,
                    w * rhs.w - (x * rhs.x + y * rhs.y + z * rhs.z));
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
  return *this = *this * rhs;
}

Quaternion Quaternion::operator*(float scalar) const {
  return Quaternion{x * scalar, y * scalar, z * scalar, w * scalar};
}

// TODO(YIDI): test this
Vector3 Quaternion::operator*(const Vector3& rhs) const {
  Quaternion quat = *this * Quaternion{rhs.x, rhs.y, rhs.z, 0} * Inverse(*this);
  return Vector3{quat.x, quat.y, quat.z};
}

Vector3 Quaternion::GetEulerAngles() const {
  Quaternion q = (*this).Normalized();

  // roll (x-axis rotation)
  float sinr = 2.f * (q.w * q.x + q.y * q.z);
  float cosr = 1.f - 2.f * (q.x * q.x + q.y * q.y);
  float roll = Util::Atan2(sinr, cosr);

  // pitch (y-axis rotation)
  float sinp = 2.f * (q.w * q.y - q.z * q.x);
  float pitch;
  if (Util::Abs(sinp) >= 1)
    pitch = Util::PI / 2 * Util::Sign(sinp);  // use 90 degrees if out of range
  else
    pitch = Util::Asin(sinp);

  // yaw (z-axis rotation)
  float siny = 2.f * (q.w * q.z + q.x * q.y);
  float cosy = 1.f - 2.f * (q.y * q.y + q.z * q.z);
  float yaw = Util::Atan2(siny, cosy);

  return Vector3{roll, pitch, yaw};
}

void Quaternion::SetFromToRotation(const Vector3& fromDirection,
                                   const Vector3& toDirection) {
  float dot = Vector3::Dot(fromDirection, toDirection);
  float k =
      Util::Sqrt(fromDirection.SqrMagnitude() * toDirection.SqrMagnitude());
  if (Util::Abs(dot / k + 1) < Util::EPSILON) {
    Vector3 ortho;
    if (fromDirection.z < fromDirection.x) {
      ortho = Vector3{fromDirection.y, -fromDirection.x, 0.f};
    } else {
      ortho = Vector3{0.f, -fromDirection.z, fromDirection.y};
    }
    *this = Quaternion(ortho.Normalized(), 0.f);
  }
  Vector3 cross = Vector3::Cross(fromDirection, toDirection);
  *this = Quaternion(cross, dot + k).Normalized();
}

void Quaternion::SetLookRotation(const Vector3& forwardDirection,
                                 const Vector3& upDirection) {
  // Normalize inputs
  Vector3 forward = forwardDirection.Normalized();
  Vector3 upwards = upDirection.Normalized();
  // Don't allow zero vectors
  if (Vector3::FuzzyEqual(forward, Vector3::zero) ||
      Vector3::FuzzyEqual(upwards, Vector3::zero)) {
    x = 0.f;
    y = 0.f;
    z = 0.f;
    w = 0.f;
  }
  // Handle alignment with up direction
  if (1 - Util::Abs(Vector3::Dot(forward, upwards)) < Util::EPSILON)
    SetFromToRotation(Vector3::forward, forward);
  // Get orthogonal vectors
  Vector3 right = Vector3::Cross(upwards, forward).Normalized();
  upwards = Vector3::Cross(forward, right);
  // Calculate rotation
  float radicand = right.x + upwards.y + forward.z;
  if (radicand > 0.f) {
    w = Util::Sqrt(1.f + radicand) * 0.5f;
    float recip = 1.f / (4.f * w);
    x = (upwards.z - forward.y) * recip;
    y = (forward.x - right.z) * recip;
    z = (right.y - upwards.x) * recip;
  } else if (right.x >= upwards.y && right.x >= forward.z) {
    x = Util::Sqrt(1.f + right.x - upwards.y - forward.z) * 0.5f;
    float recip = 1.f / (4.f * x);
    w = (upwards.z - forward.y) * recip;
    z = (forward.x + right.z) * recip;
    y = (right.y + upwards.x) * recip;
  } else if (upwards.y > forward.z) {
    y = Util::Sqrt(1.f - right.x + upwards.y - forward.z) * 0.5f;
    float recip = 1.f / (4.f * y);
    z = (upwards.z + forward.y) * recip;
    w = (forward.x - right.z) * recip;
    x = (right.y + upwards.x) * recip;
  } else {
    z = Util::Sqrt(1.f - right.x - upwards.y + forward.z) * 0.5f;
    float recip = 1.f / (4.f * z);
    y = (upwards.z + forward.y) * recip;
    x = (forward.x + right.z) * recip;
    w = (right.y - upwards.x) * recip;
  }
  Normalize();
}

Quaternion Quaternion::Normalized() const {
  Quaternion ret{x, y, z, w};
  float length = ret.x * ret.x + ret.y * ret.y + ret.z * ret.z + ret.w * ret.w;
  ret.x /= length;
  ret.y /= length;
  ret.z /= length;
  ret.w /= length;
  return ret;
}

void Quaternion::Normalize() {
  float length = x * x + y * y + z * z + w * w;
  x /= length;
  y /= length;
  z /= length;
  w /= length;
}

std::string Quaternion::ToString() const {
  std::ostringstream oss;
  oss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
  return oss.str();
}

// reference: Game Engine Architecture 2nd edition page 205
Matrix3 Quaternion::GetMatrix3() const {
  return Matrix3{1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w,
                 2 * x * z + 2 * y * w,     2 * x * y + 2 * z * w,
                 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w,
                 2 * x * z - 2 * y * w,     2 * y * z + 2 * x * w,
                 1 - 2 * x * x - 2 * y * y};
}

Quaternion Quaternion::GetInverse() const { return Inverse(*this); }

float Quaternion::Angle(const Quaternion& aQuaternion,
                        const Quaternion& bQuaternion) {
  float dot = Dot(aQuaternion, bQuaternion);
  return Util::Acos(Util::Min({Util::Abs(dot), 1.f})) * 2.f;
}

float Quaternion::Dot(const Quaternion& aQuaternion,
                      const Quaternion& bQuaternion) {
  return aQuaternion.x * bQuaternion.x + aQuaternion.y * bQuaternion.y +
         aQuaternion.z * bQuaternion.z + aQuaternion.w * bQuaternion.w;
}

Quaternion Quaternion::Inverse(const Quaternion& quaternion) {
  float length = quaternion.x * quaternion.x + quaternion.y * quaternion.y +
                 quaternion.z * quaternion.z + quaternion.w * quaternion.w;
  return Quaternion(quaternion.x / -length, quaternion.y / -length,
                    quaternion.z / -length, quaternion.w / length);
}

Quaternion Quaternion::Lerp(const Quaternion& aQuaternion,
                            const Quaternion& bQuaternion, const float t) {
  return Quaternion{aQuaternion * (1.f - t) + bQuaternion * t}.Normalized();
}

Quaternion Quaternion::Slerp(const Quaternion& aQuaternion,
                             const Quaternion& bQuaternion, float t) {
  float theta{Util::Acos(Dot(aQuaternion, bQuaternion))};
  float wp = Util::Sin(1 - t) * theta / Util::Sin(theta);
  float wq = Util::Sin(t) * theta / Util::Sin(theta);
  return aQuaternion * wp + bQuaternion * wq;
}

bool Quaternion::FuzzyEqual(const Quaternion& lhs, const Quaternion& rhs) {
  return abs(lhs.x - rhs.x) < FLT_EPSILON && abs(lhs.y - rhs.y) < FLT_EPSILON &&
         abs(lhs.z - rhs.z) < FLT_EPSILON && abs(lhs.w - rhs.w) < FLT_EPSILON;
}

Quaternion operator""_i(long double inX) {
  return Quaternion{static_cast<float>(inX), 0.f, 0.f, 0.f};
}

Quaternion operator""_j(long double inY) {
  return Quaternion{0.f, static_cast<float>(inY), 0.f, 0.f};
}

Quaternion operator""_k(long double inZ) {
  return Quaternion{0.f, 0.f, static_cast<float>(inZ), 0.f};
}

Quaternion operator""_w(long double inW) {
  return Quaternion{0.f, 0.f, 0.f, static_cast<float>(inW)};
}

}  // namespace Isetta::Math
