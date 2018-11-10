/*
 * Copyright (c) 2018 Isetta
 */
#include "AABB.h"

#include "Core/Math/Util.h"

namespace Isetta {
bool AABB::Contains(const Math::Vector3& point) const {
  return (point.x >= min.x && point.x <= max.x) &&
         (point.y >= min.y && point.y <= max.y) &&
         (point.z >= min.z && point.z <= max.z);
}
bool AABB::Contains(const AABB& aabb) const {
  return Contains(aabb.min) && Contains(aabb.max);
}

bool AABB::Intersect(const AABB& other) const {
  return (min.x <= other.max.x && max.x >= other.min.x) &&
         (min.y <= other.max.y && max.x >= other.min.x) &&
         (min.z <= other.max.z && max.z >= other.min.z);
}
bool AABB::Intersect(const AABB* a, const AABB* b) {
  return (a->min.x <= b->max.x && a->max.x >= b->min.x) &&
         (a->min.y <= b->max.y && a->max.x >= b->min.x) &&
         (a->min.z <= b->max.z && a->max.z >= b->min.z);
}

void AABB::Expand(const float amount) {
  size += amount * Math::Vector3::one;
  extents = 0.5 * size;
  min = center - extents;
  max = center + extents;
}

void AABB::Encapsulate(const Math::Vector3& point) {
  min.x = Math::Util::Min(min.x, point.x);
  min.y = Math::Util::Min(min.y, point.y);
  min.z = Math::Util::Min(min.z, point.z);
  max.x = Math::Util::Max(max.x, point.x);
  max.y = Math::Util::Max(max.y, point.y);
  max.z = Math::Util::Max(max.z, point.z);
}
void AABB::Encapsulate(const AABB& other) {
  min.x = Math::Util::Min(min.x, other.min.x);
  min.y = Math::Util::Min(min.y, other.min.y);
  min.z = Math::Util::Min(min.z, other.min.z);
  max.x = Math::Util::Max(max.x, other.max.x);
  max.y = Math::Util::Max(max.y, other.max.y);
  max.z = Math::Util::Max(max.z, other.max.z);
}
AABB AABB::Encapsulate(const AABB& a, const AABB& b) {
  return AABBConstruct(Math::Vector3{Math::Util::Min(a.min.x, b.min.x),
                                     Math::Util::Min(a.min.y, b.min.y),
                                     Math::Util::Min(a.min.z, b.min.z)},
                       Math::Vector3{Math::Util::Max(a.max.x, b.max.x),
                                     Math::Util::Max(a.max.y, b.max.y),
                                     Math::Util::Max(a.max.z, b.max.z)});
}
}  // namespace Isetta
