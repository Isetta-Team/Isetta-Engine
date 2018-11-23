/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Vector2Int.h"

#include <cmath>
#include <stdexcept>
#include "Vector2.h"

namespace Isetta::Math {

const Vector2Int Vector2Int::zero = Vector2Int();
const Vector2Int Vector2Int::one = Vector2Int(1);
const Vector2Int Vector2Int::up = Vector2Int(0, 1);
const Vector2Int Vector2Int::right = Vector2Int(1, 0);
const Vector2Int Vector2Int::down = Vector2Int(0, -1);
const Vector2Int Vector2Int::left = Vector2Int(-1, 0);

Vector2Int::Vector2Int(const Vector2& inVector)
    : x{static_cast<int>(inVector.x)}, y{static_cast<int>(inVector.y)} {}

int Vector2Int::operator[](int i) const {
  if (i < 0 || i > ELEMENT_COUNT - 1)
    throw std::out_of_range{"Vector2Int::[] => Index access out of range."};
  return xy[i];
}

float Vector2Int::Magnitude() const {
  return sqrtf(static_cast<float>(SqrMagnitude()));
}
int Vector2Int::SqrMagnitude() const { return x * x + y * y; }
Vector2 Vector2Int::Normalized() const {
  float length{Magnitude()};
  return Vector2(x / length, y / length);
}
int Vector2Int::Dot(const Vector2Int& lhs, const Vector2Int& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y;
}
int Vector2Int::Cross(const Vector2Int& lhs, const Vector2Int& rhs) {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}
float Vector2Int::Distance(const Vector2Int& start, const Vector2Int& end) {
  return (start - end).Magnitude();
}
Vector2Int Vector2Int::Scale(const Vector2Int& inVector,
                             const Vector2Int& scalar) {
  return Vector2Int(inVector.x * scalar.x, inVector.y * scalar.y);
}
}  // namespace Isetta::Math
