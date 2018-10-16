/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Math/Rect.h"

#include "Vector2.h"

namespace Isetta::Math {
Math::Rect::Rect(const Vector2& position, const Vector2& size)
    : x{position.x}, y{position.y}, width{size.x}, height{size.y} {}

Vector2 Math::Rect::Position() const { return Vector2{x, y}; }
Vector2 Rect::Center() const { return Vector2{x + width / 2, y + height / 2}; }
Vector2 Rect::Min() const { return Vector2{x, y}; }
Vector2 Rect::Max() const { return Vector2{x + width, y + height}; }
Vector2 Rect::Size() const { return Vector2{width, height}; }
float Rect::Area() const { return height * width; }

bool Rect::Contains(const Vector2& point) const {
  return point.x > x && point.y > y && point.x < x + width &&
         point.y < y + height;
}
bool Rect::Overlaps(const Rect& rhs) const {
  return Contains(rhs.Position()) || rhs.Contains(Position());
}

Vector2 Rect::NormalizedToPoint(const Rect& rect,
                                const Vector2& normalizedCoord) {
  return Vector2{rect.x + normalizedCoord.x * rect.width,
                 rect.y + normalizedCoord.y * rect.height};
}
Vector2 Rect::PointToNormalized(const Rect& rect, const Vector2& point) {
  return Vector2{(point.x - rect.x) / rect.width,
                 (point.y - rect.y) / rect.height};
}

bool Rect::FuzzyEqual(const Rect& lhs, const Rect& rhs) {
  return abs(lhs.x - rhs.x) < FLT_EPSILON && abs(lhs.y - rhs.y) < FLT_EPSILON &&
         abs(lhs.width - rhs.width) < FLT_EPSILON && abs(lhs.height - rhs.height) < FLT_EPSILON;
}

}  // namespace Isetta::Math
