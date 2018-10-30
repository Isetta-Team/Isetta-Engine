/*
 * Copyright (c) 2018 Isetta
 */
#include "AI/Nav2DPlane.h"
#include "Core/Math/Matrix4.h"
#include "Core/Memory/MemoryManager.h"

using namespace Isetta;

template <typename T>
Nav2DPlane::Grid2D<T>::Grid2D(int width, int height)
    : data{static_cast<T*>(
          MemoryManager::AllocOnFreeList(width * height * sizeof(T)))},
      rows{height},
      columns{width} {
  memset(data, 0, width * height * sizeof(T));
}

template <typename T>
T& Nav2DPlane::Grid2D<T>::operator()(size_t row, size_t column) const {
  return data[row * columns + column];
}

Nav2DPlane::Nav2DPlane(const Math::Rect& gridSurface,
                       const Math::Vector2Int& divideNums)
    : costGrid{divideNums.x, divideNums.y},
      vectorGrid{divideNums.x, divideNums.y},
      surface{gridSurface},
      divideInfo{divideNums},
      nodeSize{gridSurface.width / divideNums.x,
               gridSurface.height / divideNums.y} {}

void Nav2DPlane::DebugDisplay() {
  Math::Matrix4 currentLocation{Math::Matrix4::Translate(Math::Vector3{
      surface.x + nodeSize.x / 2, 0, surface.y + nodeSize.y / 2})};

}
