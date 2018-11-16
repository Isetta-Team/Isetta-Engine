/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector2.h"
#include "Core/DataStructures/Array.h"

struct Nav2DObstacle {
  Isetta::Array<Isetta::Math::Vector2> obstaclePoints;
  Nav2DObstacle() = default;
  explicit Nav2DObstacle(const Isetta::Array<Isetta::Math::Vector2>& points);
};
