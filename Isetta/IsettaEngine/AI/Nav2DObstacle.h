/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/Math/Vector2.h"

namespace Isetta {
struct Nav2DObstacle {
  Array<Math::Vector2> obstaclePoints;
  Nav2DObstacle() = default;
  explicit Nav2DObstacle(const Array<Math::Vector2>& points);
};
}  // namespace Isetta
