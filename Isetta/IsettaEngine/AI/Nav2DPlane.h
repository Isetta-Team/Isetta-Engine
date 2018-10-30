/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2Int.h"
#include "Core/math/Vector2.h"

namespace Isetta {
class Nav2DPlane {
  template <typename T>
  class Grid2D {
    T* data;
    size_t rows;
    size_t columns;

   public:
    Grid2D(int width, int height);
    T& operator()(size_t row, size_t column) const;
  };

  Grid2D<U8> costGrid;
  Grid2D<Math::Vector2Int> vectorGrid;

  Math::Rect surface;
  Math::Vector2Int divideInfo;
  Math::Vector2 nodeSize;

 public:
  Nav2DPlane(const Math::Rect& gridSurface, const Math::Vector2Int& divideNums);
  void DebugDisplay();
};
}  // namespace Isetta
