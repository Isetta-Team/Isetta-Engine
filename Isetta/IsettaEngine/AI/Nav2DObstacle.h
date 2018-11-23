/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/Math/Vector2.h"
#include "ISETTA_API.h"

namespace Isetta {
struct ISETTA_API Nav2DObstacle {
  Array<Math::Vector2> obstaclePoints;
  Nav2DObstacle() = default;
  explicit Nav2DObstacle(const Array<Math::Vector2>& points);
};
}  // namespace Isetta
