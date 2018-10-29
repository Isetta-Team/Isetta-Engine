#pragma once
#include "Collisions/Collider.h"
#include <unordered_set>

namespace Isetta::CollisionUtil {
struct ColliderPairHasher {
  size_t operator()(const std::pair<Collider *, Collider *> pair) const {
    return std::hash<void *>()(pair.first) ^ std::hash<void *>()(pair.second);
  }
};

using CollisionPair = std::pair<Collider*, Collider*>;
using ColliderPairSet = std::unordered_set<CollisionPair, ColliderPairHasher>;

}
