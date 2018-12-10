/*
 * Copyright (c) 2018 Isetta
 */
#include "AI/Nav2DObstacle.h"

#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Util.h"
#include "Core/Math/Vector2.h"

#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/SphereCollider.h"

namespace Isetta {
Nav2DObstacle::Nav2DObstacle(const Isetta::Array<Isetta::Math::Vector2>& points)
    : points{points} {}

Nav2DObstacle Nav2DObstacle::Rectangle(const Math::Rect& rect) {
  return Nav2DObstacle{
      {rect.Min(), rect.Min() + rect.width * Math::Vector2::right, rect.Max(),
       rect.Min() + rect.height * Math::Vector2::up}};
}

Nav2DObstacle Nav2DObstacle::Circle(const Math::Vector2& position, float radius,
                                    float angleOffset, int segments) {
  Nav2DObstacle obstacle;
  obstacle.points.Reserve(segments);
  float angle = 2 * Math::Util::PI / segments;
  for (int i = 0; i < segments; ++i) {
    float x = radius * Math::Util::Cos(i * angle + angleOffset);
    float y = radius * Math::Util::Sin(i * angle + angleOffset);
    obstacle.points.PushBack(Math::Vector2{x, y} + position);
  }
  return obstacle;
}
#ifdef _EDITOR
void Nav2DObstacle::DebugDraw() const {
  for (int i = 0; i < points.Size(); ++i) {
    DebugDraw::Line({points[i].x, 0, points[i].y},
                    {points[(i + 1) % points.Size()].x, 0,
                     points[(i + 1) % points.Size()].y},
                    Color::brown, 1, 0, true);
  }
}
#endif

// Nav2DObstacle::Nav2DObstacle(const BoxCollider& collider, bool useYPos) {
//  points.Reserve(4);
//  Math::Vector3 extents = collider.GetWorldExtents();
//  Math::Vector3 pos = collider.GetWorldCenter();
//
//  points.PushBack() collider.GetWorldCenter() + extents;
//  collider.GetWorldCenter() - extents;
//}

// Nav2DObstacle::Nav2DObstacle(const SphereCollider& collider, bool useYPos) {}
//
// Nav2DObstacle::Nav2DObstacle(const CapsuleCollider& collider, bool useYPos)
// {}
}  // namespace Isetta
