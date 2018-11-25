/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Color.h"
#include "Core/DataStructures/Array.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2Int.h"
#include "Core/math/Vector2.h"
#include "Nav2DObstacle.h"
#include "ISETTA_API.h"

namespace Isetta {
class ISETTA_API Nav2DPlane {
  Array<U8> costMatrix;
  Array<Math::Vector2> dirMatrix;
  Array<bool> isObstacle;

  Math::Rect surface;
  Math::Vector2Int divideInfo;
  Math::Vector2 nodeSize;

  Array<Color> colors{Color::white, Color::yellow,    Color::orange,
                      Color::red,   Color::magenta,   Color::blue,
                      Color::cyan,  Color::lightGrey, Color::brown};
  Array<Nav2DObstacle> obstacles;

  Math::Vector2Int GetIndexByPosition(Math::Vector2 position) const;
  Array<Math::Vector2Int> targetIndices;
  Array<class Transform*> currTargets;
  inline int Vector2IndexToInt(Math::Vector2Int index) const;
  inline int Vector2IndexToInt(int x, int y) const;
  void UpdateCost(Math::Vector2Int targetIndex);
  void UpdateDirection();
  bool IsIndexUnavailable(Math::Vector2Int index) const;
  void AddLine(Math::Vector2 start, Math::Vector2 end);

 public:
  Nav2DPlane() = default;
  Nav2DPlane(const Math::Rect& gridSurface, const Math::Vector2Int& divideNums);
#ifdef _EDITOR
  void DebugDisplay() const;
#endif
  void AddTarget(class Transform* transform);
  void RemoveTarget(class Transform* transform);
  void UpdateRoute();
  void AddObstacle(const Nav2DObstacle& obstacle);
  Math::Vector2 GetDirectionByPosition(Math::Vector2 position);
  float GetDistanceToTarget(Math::Vector2 position) const;
};
}  // namespace Isetta
