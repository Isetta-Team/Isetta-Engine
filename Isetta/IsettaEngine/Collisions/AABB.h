/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Util.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
class AABB {
 private:
  Math::Vector3 center, size, extents, min, max;

  static inline AABB AABBConstruct(const Math::Vector3& min,
                                   const Math::Vector3& max) {
    Math::Vector3 center = 0.5f * (max + min);
    Math::Vector3 size = 2 * (max - center);
    return AABB(center, size);
  }

 public:
  inline AABB(const Math::Vector3& center, const Math::Vector3& size)
      : center{center}, size{size} {
    extents = 0.5 * size;
    min = center - extents;
    max = center + extents;
  }

  inline Math::Vector3 GetCenter() const { return center; }
  inline Math::Vector3 GetSize() const { return size; }
  inline Math::Vector3 GetExtents() const { return extents; }
  inline Math::Vector3 GetMin() const { return min; }
  inline Math::Vector3 GetMax() const { return max; }

  inline bool Contains(const Math::Vector3& point) const {
    return (point.x >= min.x && point.x <= max.x) &&
           (point.y >= min.y && point.y <= max.y) &&
           (point.z >= min.z && point.z <= max.z);
  }

  inline bool Intersect(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.x >= other.min.x) &&
           (min.z <= other.max.z && max.z >= other.min.z);
  }

  static inline bool Intersect(const AABB* a, const AABB* b) {
    return (a->min.x <= b->max.x && a->max.x >= b->min.x) &&
           (a->min.y <= b->max.y && a->max.x >= b->min.x) &&
           (a->min.z <= b->max.z && a->max.z >= b->min.z);
  }

  inline void Expand(const float amount) {
    size += amount * Math::Vector3::one;
    extents = 0.5 * size;
    min = center - extents;
    max = center + extents;
  }

  inline void Encapsulate(const Math::Vector3& point) {
    min.x = Math::Util::Min(min.x, point.x);
    min.y = Math::Util::Min(min.y, point.y);
    min.z = Math::Util::Min(min.z, point.z);
    max.x = Math::Util::Max(max.x, point.x);
    max.y = Math::Util::Max(max.y, point.y);
    max.z = Math::Util::Max(max.z, point.z);
  }

  inline void Encapsulate(const AABB& other) {
    min.x = Math::Util::Min(min.x, other.min.x);
    min.y = Math::Util::Min(min.y, other.min.y);
    min.z = Math::Util::Min(min.z, other.min.z);
    max.x = Math::Util::Max(max.x, other.max.x);
    max.y = Math::Util::Max(max.y, other.max.y);
    max.z = Math::Util::Max(max.z, other.max.z);
  }

  inline static AABB Encapsulate(const AABB& a, const AABB& b) {
    return AABBConstruct(Math::Vector3{Math::Util::Min(a.min.x, b.min.x),
                                       Math::Util::Min(a.min.y, b.min.y),
                                       Math::Util::Min(a.min.z, b.min.z)},
                         Math::Vector3{Math::Util::Max(a.max.x, b.max.x),
                                       Math::Util::Max(a.max.y, b.max.y),
                                       Math::Util::Max(a.max.z, b.max.z)});
  }

  float SurfaceArea() const { return 2 * (size.x * size.y + size.y * size.z + size.x * size.z); }
  // Math::Vector3 ClosestPoint(Math::Vector3 point);
  // void IntersectRay(Ray ray);
};

// class Sphere {
// public:
//  Math::Vector4 sphere;
//
//  inline bool PointInside(Math::Vector3 point) {
//    return (point - sphere.GetVector3()).SqrMagnitude() <= sphere.w *
//    sphere.w;
//  }
//
//  inline bool Intersect(const Sphere& other) {
//    return (other.sphere.GetVector3() - sphere.GetVector3()).SqrMagnitude() <
//           (other.sphere.w + sphere.w) * (other.sphere.w + sphere.w);
//  }
//};

// AABB* Intersect(const BVNode* node, const AABB* other) {
//  if (node->left && node->left->data->Intersect(*other)) {
//    AABB* ret = Intersect(node->left, other);
//    return ret ? ret : Intersect(node->left, other);
//  } else if (node->right && node->right->data->Intersect(*other)) {
//    AABB* ret = Intersect(node->left, other);
//    return ret ? ret : Intersect(node->left, other);
//  }
//  return nullptr;
//}

}  // namespace Isetta
