// "Copyright [2018] Isetta"

#include "Vector4.h"

#include <cfloat>
#include <cmath>
#include <stdexcept>
#include "Core/Color.h"

namespace Isetta::Math {

const Vector4 Vector4::zero = Vector4();
const Vector4 Vector4::one = Vector4(1.f);

Vector4(const Color& c) : x{c.r}, y{c.g}, z{c.b}, w{c.a} {}

float Vector4::operator[](int i) const {
  switch (i) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
    default:
      throw std::logic_error;
  }
}

float Vector4::Magnitude() const { return sqrtf(SqrMagnitude()); }
float Vector4::SqrMagnitude() const { return x * x + y * y + z * z + w * w; }
Vector4 Vector4::Normalized() const {
  float length{Magnitude()};
  return Vector4(x / length, y / length, z / length, w / length);
}
void Vector4::Normalize() noexcept {
  float length{Magnitude()};
  x /= length;
  y /= length;
  z /= length;
  w /= length;
}
bool Vector4::Equals(const Vector4& lhs, const Vector4& rhs) {
  return abs(lhs.x - rhs.x) < FLT_EPSILON && abs(lhs.y - rhs.y) < FLT_EPSILON &&
         abs(lhs.z - rhs.z) < FLT_EPSILON && abs(lhs.w - rhs.w) < FLT_EPSILON;
}
float Vector4::Dot(const Vector4& lhs, const Vector4& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}
Vector4 Vector4::Lerp(const Vector4& start, const Vector4& end, float time) {
  return start * (1.f - time) + end * time;
}
float Vector4::Distance(const Vector4& start, const Vector4& end) {
  return (start - end).Magnitude();
}
Vector4 Vector4::Project(const Vector4& inVector, const Vector4& onNormal) {
  return onNormal.Normalized() * Dot(inVector, onNormal);
}
Vector4 Vector4::Scale(const Vector4& inVector, const Vector4& scalar) {
  return Vector4(inVector.x * scalar.x, inVector.y * scalar.y,
                 inVector.z * scalar.z, inVector.w * scalar.w);
}
Vector4 Vector4::Slerp(const Vector4& start, const Vector4& end, float time) {
  float dot = Dot(start, end);
  dot = dot < -1.f ? -1.f : (dot > 1.f ? 1.f : dot);
  float theta = acosf(dot) * time;
  Vector4 relativeVector = end - start * dot;
  return start * cosf(theta) + relativeVector * sinf(theta);
}
}  // namespace Isetta::Math
