/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Vector3Int.h"

#include <cmath>
#include <stdexcept>
#include "Core/Math/Vector2Int.h"
#include "Core/Math/Vector3.h"

namespace Isetta::Math {

const Vector3Int Vector3Int::zero = Vector3Int();
const Vector3Int Vector3Int::one = Vector3Int(1);
const Vector3Int Vector3Int::up = Vector3Int(0, 1, 0);
const Vector3Int Vector3Int::right = Vector3Int(1, 0, 0);
const Vector3Int Vector3Int::forward = Vector3Int(0, 0, 1);
const Vector3Int Vector3Int::down = Vector3Int(0, -1, 0);
const Vector3Int Vector3Int::left = Vector3Int(-1, 0, 0);
const Vector3Int Vector3Int::back = Vector3Int(0, 0, -1);

Vector3Int::Vector3Int(const Vector2Int& inVector, int inZ)
    : x{inVector.x}, y{inVector.y}, z{inZ} {}

Vector3Int::Vector3Int(const Vector3& inVector)
    : x{static_cast<int>(inVector.x)},
      y{static_cast<int>(inVector.y)},
      z{static_cast<int>(inVector.z)} {}

Vector3Int::operator class Vector2Int() { return Vector2Int(x, y); }

int Vector3Int::operator[](int i) const {
  if (i < 0 || i > ELEMENT_COUNT - 1)
    throw std::out_of_range{"Vector3Int::[] => Index access out of range."};
  return xyz[i];
}

float Vector3Int::Magnitude() const {
  return sqrtf(static_cast<float>(SqrMagnitude()));
}
int Vector3Int::SqrMagnitude() const { return x * x + y * y + z * z; }
Vector3 Vector3Int::Normalized() const {
  float length{Magnitude()};
  return Vector3(x / length, y / length, z / length);
}
int Vector3Int::Dot(const Vector3Int& lhs, const Vector3Int& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
Vector3Int Vector3Int::Cross(const Vector3Int& lhs, const Vector3Int& rhs) {
  return Vector3Int(lhs.y * rhs.z - lhs.z * rhs.y,
                    -lhs.x * rhs.z + lhs.z * rhs.x,
                    lhs.x * rhs.y - lhs.y * rhs.x);
}
float Vector3Int::Distance(const Vector3Int& start, const Vector3Int& end) {
  return (start - end).Magnitude();
}
Vector3Int Vector3Int::Scale(const Vector3Int& aVector,
                             const Vector3Int& bVector) {
  return Vector3Int(aVector.x * bVector.x, aVector.y * bVector.y,
                    aVector.z * bVector.z);
}
}  // namespace Isetta::Math
