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

  inline bool Contains(Math::Vector3 point) {
    return (point.x >= min.x && point.x <= max.x) &&
           (point.y >= min.y && point.y <= max.y) &&
           (point.z >= min.z && point.z <= max.z);
  }

  inline bool Intersect(const AABB& other) {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.x >= other.min.x) &&
           (min.z <= other.max.z && max.z >= other.min.z);
  }
  static inline bool Intersect(const AABB* a, const AABB* b) {
    return (a->min.x <= b->max.x && a->max.x >= b->min.x) &&
           (a->min.y <= b->max.y && a->max.x >= b->min.x) &&
           (a->min.z <= b->max.z && a->max.z >= b->min.z);
  }

  inline void Expand(float amount) {
    size += amount * Math::Vector3::one;
    extents = 0.5 * size;
    min = center - extents;
    max = center + extents;
  }

  inline void Encapsulate(Math::Vector3 point) {
    min.x = Math::Util::Min(min.x, point.x);
    min.y = Math::Util::Min(min.y, point.y);
    min.z = Math::Util::Min(min.z, point.z);
    max.x = Math::Util::Min(max.x, point.x);
    max.y = Math::Util::Min(max.y, point.y);
    max.z = Math::Util::Min(max.z, point.z);
  }
  inline void Encapsulate(const AABB& other) {
    min.x = Math::Util::Min(min.x, other.min.x);
    min.y = Math::Util::Min(min.y, other.min.y);
    min.z = Math::Util::Min(min.z, other.min.z);
    max.x = Math::Util::Min(max.x, other.max.x);
    max.y = Math::Util::Min(max.y, other.max.y);
    max.z = Math::Util::Min(max.z, other.max.z);
  }
  inline static AABB Encapsulate(const AABB& a, const AABB& b) {
    return AABBConstruct(Math::Vector3{Math::Util::Min(a.min.x, b.min.x),
                                       Math::Util::Min(a.min.y, b.min.y),
                                       Math::Util::Min(a.min.z, b.min.z)},
                         Math::Vector3{Math::Util::Min(a.max.x, b.max.x),
                                       Math::Util::Min(a.max.y, b.max.y),
                                       Math::Util::Min(a.max.z, b.max.z)});
  }

  float SurfaceArea() const { return 2 * size.SqrMagnitude(); }

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

struct BVNode {
  AABB* aabb;
  BVNode *left, *right;

  BVNode(AABB* val) {
    aabb = nullptr;
    left = right = nullptr;
  }

  bool IsLeaf() const { return left == nullptr && right == nullptr; }
};

class BVTree {
 private:
  BVNode* root;

 public:
  BVTree() { root = nullptr; }

  inline BVNode* RotationRR(BVNode* parent) {
    BVNode* tmp = parent->right;
    parent->right = tmp->left;
    tmp->left = parent;
    return tmp;
  }
  inline BVNode* RotationRL(BVNode* parent) {
    BVNode* tmp = parent->right;
    parent->right = RotationLL(tmp);
    return RotationRR(parent);
  }
  inline BVNode* RotationLR(BVNode* parent) {
    BVNode* tmp = parent->left;
    parent->left = RotationRR(tmp);
    return RotationLL(parent);
  }
  inline BVNode* RotationLL(BVNode* parent) {
    BVNode* tmp = parent->left;
    parent->left = tmp->right;
    tmp->right = parent;
    return tmp;
  }

  inline BVNode* Balance(BVNode* node) {  // TODO
    return node;
  }
  inline BVNode* Insert(BVNode* node, BVNode* parent, AABB* data) {
    if (node == nullptr) {
      node = new BVNode(data);
      if (parent) {
        // parent->aabb->Encapsulate(*node->aabb);
        BVNode* tmp = parent;
      }
      return node;
    }
    if (AABB::Encapsulate(*node->left->aabb, *data).SurfaceArea() -
            node->left->aabb->SurfaceArea() <
        AABB::Encapsulate(*node->right->aabb, *data).SurfaceArea() -
            node->right->aabb->SurfaceArea()) {
      node->left = Insert(node->left, node, data);
      node = Balance(node);
    } else {
      node->right = Insert(node->right, node, data);
      node = Balance(node);
    }
    return node;
  }
  inline BVNode* Add(AABB* data) { return Insert(root, nullptr, data); }
};  // namespace Isetta

}  // namespace Isetta
