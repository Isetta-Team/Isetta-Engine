/*
 * Copyright (c) 2018 Isetta
 */
#include "AI/Nav2DObstacle.h"

#include "Core/Math/Rect.h"
#include "Core/Math/Util.h"
#include "Core/Math/Vector2.h"

#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/SphereCollider.h"

namespace Isetta {
Nav2DObstacle::Nav2DObstacle(const Isetta::Array<Isetta::Math::Vector2>& points)
    : obstaclePoints{points} {}

Nav2DObstacle Nav2DObstacle::Rectangle(const Math::Rect& rect) {
  return Nav2DObstacle{
      {rect.Min(), rect.Min() + rect.width * Math::Vector2::left, rect.Max(),
       rect.Min() + rect.height * Math::Vector2::up}};
}

Nav2DObstacle Nav2DObstacle::Circle(const Math::Vector2& position, float radius,
                                    int segments) {
  Nav2DObstacle obstacle;
  obstacle.obstaclePoints.Reserve(segments);
  float angle = 2 * Math::Util::PI / segments;
  for (int i = 0; i < segments; ++i) {
    float x = radius * Math::Util::Cos(i * angle);
    float y = radius * Math::Util::Sin(i * angle);
    obstacle.obstaclePoints.PushBack(Math::Vector2{x, y} + position);
  }
  return obstacle;
}

// Nav2DObstacle::Nav2DObstacle(const BoxCollider& collider, bool useYPos) {
//  obstaclePoints.Reserve(4);
//  Math::Vector3 extents = collider.GetWorldExtents();
//  Math::Vector3 pos = collider.GetWorldCenter();
//
//  obstaclePoints.PushBack() collider.GetWorldCenter() + extents;
//  collider.GetWorldCenter() - extents;
//}

// Nav2DObstacle::Nav2DObstacle(const SphereCollider& collider, bool useYPos) {}
//
// Nav2DObstacle::Nav2DObstacle(const CapsuleCollider& collider, bool useYPos)
// {}
}  // namespace Isetta
