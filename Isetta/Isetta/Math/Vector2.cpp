// "Copyright [2018] Isetta"

#include "Vector2.h"

#include <cfloat>
#include <cmath>
#include "Vector3Int.h"

namespace Isetta::Math {

const Vector2 Vector2::zero = Vector2();
const Vector2 Vector2::one = Vector2(1.f);
const Vector2 Vector2::up = Vector2(0.f, 1.f);
const Vector2 Vector2::right = Vector2(1.f, 0.f);
const Vector2 Vector2::down = Vector2(0.f, -1.f);
const Vector2 Vector2::left = Vector2(-1.f, 0.f);

Vector2::Vector2(const Vector3Int& inIntVector)
    : x{float(inIntVector.x)}, y{float(inIntVector.y)} {}

float Vector2::Magnitude() const { return sqrtf(SqrMagnitude()); }
float Vector2::SqrMagnitude() const { return x * x + y * y; }
Vector2 Vector2::Normalized() const {
  float length{Magnitude()};
  return Vector2(x / length, y / length);
}
void Vector2::Normalize() noexcept {
  float length{Magnitude()};
  x /= length;
  y /= length;
}
bool Vector2::Equals(const Vector2& lhs, const Vector2& rhs) {
  return abs(lhs.x - rhs.x) < FLT_EPSILON && abs(lhs.y - rhs.y) < FLT_EPSILON;
}
float Vector2::Dot(const Vector2& lhs, const Vector2& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}
float Vector2::Cross(const Vector2& lhs, const Vector2& rhs) {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}
Vector2 Vector2::Lerp(const Vector2& start, const Vector2& end, float time) {
  return start * (1.f - time) + end * time;
}
float Vector2::Distance(const Vector2& start, const Vector2& end) {
  return (start - end).Magnitude();
}
Vector2 Vector2::Project(const Vector2& inVector, const Vector2& onNormal) {
  return onNormal.Normalized() * Dot(inVector, onNormal);
}
Vector2 Vector2::Reflect(const Vector2& inVector, const Vector2& inNormal) {
  Vector2 normal{inNormal.Normalized()};
  return inVector - normal * Dot(inVector, normal) * 2.f;
}
Vector2 Vector2::Scale(const Vector2& inVector, const Vector2& scalar) {
  return Vector2(inVector.x * scalar.x, inVector.y * scalar.y);
}
Vector2 Vector2::Slerp(const Vector2& start, const Vector2& end, float time) {
  float dot = Dot(start, end);
  dot = dot < -1.f ? -1.f : (dot > 1.f ? 1.f : dot);
  float theta = acosf(dot) * time;
  Vector2 relativeVector = end - start * dot;
  return start * cosf(theta) + relativeVector * sinf(theta);
}
}  // namespace Math
