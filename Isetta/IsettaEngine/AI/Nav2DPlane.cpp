/*
 * Copyright (c) 2018 Isetta
 */
#include "AI/Nav2DPlane.h"
#include <queue>
#include "Core/Debug/DebugDraw.h"
#include "Core/Math/Vector3.h"
#include "Scene/Transform.h"

using namespace Isetta;

Math::Vector2Int Nav2DPlane::GetIndexByPosition(Math::Vector2 position) const {
  return Math::Vector2Int{
      Math::Util::FloorToInt((position.x - surface.x) / nodeSize.x),
      Math::Util::FloorToInt((position.y - surface.y) / nodeSize.y)};
}

int Nav2DPlane::Vector2IndexToInt(Math::Vector2Int index) const {
  return index.y * divideInfo.x + index.x;
}

int Nav2DPlane::Vector2IndexToInt(int x, int y) const {
  return y * divideInfo.x + x;
}

void Nav2DPlane::UpdateCost(Math::Vector2Int targetIndex) {
  Array<bool> visited(divideInfo.x * divideInfo.y, false);
  std::queue<Math::Vector2Int> updatingQueue;
  visited[Vector2IndexToInt(targetIndex)] = true;
  costMatrix[Vector2IndexToInt(targetIndex)] = 0;
  updatingQueue.push(targetIndex);
  auto checkAndQueueNede = [&](Math::Vector2Int nextIndex, int currFlatIndex) {
    if (IsIndexUnavailable(nextIndex)) return;
    int flatNextIndex{Vector2IndexToInt(nextIndex)};
    if (!visited[flatNextIndex]) {
      costMatrix[flatNextIndex] = Math::Util::Min(costMatrix[currFlatIndex] + 1,
                                                  costMatrix[flatNextIndex]);
      visited[flatNextIndex] = true;
      updatingQueue.push(nextIndex);
    }
  };
  while (updatingQueue.size() > 0) {
    Math::Vector2Int currIndex = updatingQueue.front();
    int flatIndex = Vector2IndexToInt(currIndex);
    updatingQueue.pop();
    Math::Vector2Int nextIndex{currIndex + Math::Vector2Int::left};
    checkAndQueueNede(nextIndex, flatIndex);
    nextIndex = {currIndex + Math::Vector2Int::down};
    checkAndQueueNede(nextIndex, flatIndex);
    nextIndex = {currIndex + Math::Vector2Int::right};
    checkAndQueueNede(nextIndex, flatIndex);
    nextIndex = {currIndex + Math::Vector2Int::up};
    checkAndQueueNede(nextIndex, flatIndex);
  }
}

void Nav2DPlane::UpdateDirection() {
  int index = 0;
  auto costFunc = [&](Math::Vector2Int curr, Math::Vector2Int dir) {
    if (IsIndexUnavailable(curr + dir)) {
      dir = Math::Vector2Int::zero;
    }
    return costMatrix[Vector2IndexToInt(curr + dir)];
  };
  for (int i = 0; i < divideInfo.y; ++i) {
    for (int j = 0; j < divideInfo.x; ++j) {
      int minCost{255};
      float minX{0.0}, minY{0.0};
      for (int x = -1; x < 2; ++x) {
        for (int y = -1; y < 2; ++y) {
          if (IsIndexUnavailable({j + x, i}) ||
              IsIndexUnavailable({j, i + y})) {
            continue;
          }
          Math::Vector2Int dir = Math::Vector2Int{x, y};
          int cost = costFunc({j, i}, dir);
          if (cost < minCost) {
            minCost = cost;
            minX = x;
            minY = y;
          }
        }
      }
      dirMatrix[index++] = Math::Vector2{minX, minY}.Normalized();
    }
  }
}

bool Nav2DPlane::IsIndexUnavailable(Math::Vector2Int index) const {
  return index.x < 0 || index.y < 0 || index.x >= divideInfo.x ||
         index.y >= divideInfo.y || isObstacle[Vector2IndexToInt(index)];
}

Nav2DPlane::Nav2DPlane(const Math::Rect& gridSurface,
                       const Math::Vector2Int& divideNums)
    : costMatrix(divideNums.x * divideNums.y, 255),
      dirMatrix(divideNums.x * divideNums.y, Math::Vector2::zero),
      isObstacle(divideNums.x * divideNums.y, false),
      surface{gridSurface},
      divideInfo{divideNums},
      nodeSize{gridSurface.width / divideNums.x,
               gridSurface.height / divideNums.y} {}

#ifdef _EDITOR
void Nav2DPlane::DebugDraw() const {
  const Math::Vector2 startingPoint{surface.Min() + 0.5 * nodeSize};
  int index = -1;
  for (int i = 0; i < divideInfo.y; ++i) {
    for (int j = 0; j < divideInfo.x; ++j) {
      ++index;
      Math::Vector3 position{startingPoint.x + j * nodeSize.x, 0,
                             startingPoint.y + i * nodeSize.y};
      if (isObstacle[index] || costMatrix[index] == 255) {
        DebugDraw::WireCube(
            Math::Matrix4::Transform(position, Math::Vector3::zero,
                                     Math::Vector3::one * 0.1),
            colors[8], 1, 0, false);
        continue;
      }
      DebugDraw::WireCube(
          Math::Matrix4::Transform(position, Math::Vector3::zero,
                                   Math::Vector3::one * 0.1),
          colors[Math::Util::Clamp(0, 7, costMatrix[index] / 3)], 1, 0, false);
      DebugDraw::Line(position,
                      position + 0.25 * Math::Vector3{dirMatrix[index].x, 0,
                                                      dirMatrix[index].y},
                      Color::white, 1, 0, false);
    }
  }
  for (const auto& obstacle : obstacles) {
    obstacle.DebugDraw();
  }
}
#endif

void Nav2DPlane::AddTarget(class Transform* transform) {
  currTargets.PushBack(transform);
  // SetTargetNode(GetIndexByPosition(position));
}

void Nav2DPlane::RemoveTarget(Transform* transform) {
  Array<Transform*>::iterator targetIter =
      std::find(currTargets.begin(), currTargets.end(), transform);
  if (targetIter == currTargets.end()) {
    throw std::logic_error("Nav2DPlane:RemoveTarget => No target found");
  } else {
    currTargets.Erase(targetIter);
  }
}

void Nav2DPlane::UpdateRoute() {
  costMatrix.Assign(costMatrix.Size(), 255);
  for (const auto transform : currTargets) {
    Math::Vector2 position{transform->GetWorldPos().x,
                           transform->GetWorldPos().z};
    if (surface.Contains(position)) {
      UpdateCost(GetIndexByPosition(position));
    }
  }
  UpdateDirection();
}

void Nav2DPlane::AddLine(Math::Vector2 start, Math::Vector2 end) {
  auto setObstacle = [&](Math::Vector2 point) {
    if (surface.Contains(point)) {
      isObstacle[Vector2IndexToInt(GetIndexByPosition(point))] = true;
    }
  };
  if (start.x == end.x) {
    if (start.y > end.y) {
      std::swap(start, end);
    }
    int steps = Math::Util::CeilToInt(
        (Math::Util::CeilToInt(end.y) - Math::Util::FloorToInt(start.y)) /
        nodeSize.y);
    for (int i = 0; i <= steps; ++i) {
      Math::Vector2 point{start + i * nodeSize.y * Math::Vector2::up};
      setObstacle(point);
    }
  } else if (start.y == end.y) {
    if (start.x > end.x) {
      std::swap(start, end);
    }
    int steps = Math::Util::CeilToInt(
        (Math::Util::CeilToInt(end.x) - Math::Util::FloorToInt(start.x)) /
        nodeSize.x);
    for (int i = 0; i <= steps; ++i) {
      Math::Vector2 point{start + i * nodeSize.x * Math::Vector2::right};
      setObstacle(point);
    }
  } else {
    float sx = Math::Util::Min(start.x, end.x);
    float sy = Math::Util::Min(start.y, end.y);
    float ex = Math::Util::Max(start.x, end.x);
    float ey = Math::Util::Max(start.y, end.y);
    float stepX =
        (Math::Util::CeilToInt(ex) - Math::Util::FloorToInt(sx)) / nodeSize.x;
    float stepY =
        (Math::Util::CeilToInt(ey) - Math::Util::FloorToInt(sy)) / nodeSize.y;
    int steps{0};
    if ((ex - sx) > (ey - sy)) {
      steps = Math::Util::CeilToInt(stepX);
      if (start.x > end.x) {
        std::swap(start, end);
      }
    } else {
      steps = Math::Util::CeilToInt(stepY);
      if (start.y > end.y) {
        std::swap(start, end);
      }
    }
    float stepSizeX = (end.x - start.x) / steps;
    float stepSizeY = (end.y - start.y) / steps;
    Math::Vector2 step{stepSizeX, stepSizeY};
    for (int i = 0; i <= steps; ++i) {
      Math::Vector2 point{start + i * step};
      setObstacle(point);
    }
  }
}

void Nav2DPlane::AddObstacle(const Nav2DObstacle& obstacle) {
  obstacles.PushBack(obstacle);
  for (int i = 0; i < obstacle.points.Size(); ++i) {
    AddLine(obstacle.points[i],
            obstacle.points[(i + 1) % obstacle.points.Size()]);
  }
}

Math::Vector2 Nav2DPlane::GetDirectionByPosition(Math::Vector2 position) {
  Math::Vector2Int index{GetIndexByPosition(position)};
  if (IsIndexUnavailable(index)) return Math::Vector2::zero;
  return dirMatrix[Vector2IndexToInt(index)];
}

std::tuple<float, Transform*> Nav2DPlane::GetDistanceToTarget(
    Math::Vector2 position) const {
  float distance{surface.width * surface.height};
  Transform* minTarget = nullptr;
  for (auto transform : currTargets) {
    float currDistance = Math::Vector2::Distance(
        position, {transform->GetWorldPos().x, transform->GetWorldPos().z});
    if (currDistance < distance) {
      distance = currDistance;
      minTarget = transform;
    }
  }
  return {distance, minTarget};
}
