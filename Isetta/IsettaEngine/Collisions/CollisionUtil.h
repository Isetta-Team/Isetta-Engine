#pragma once
#include <unordered_set>
#include "Util.h"

namespace Isetta::CollisionUtil {
using CollisionPair = std::pair<Collider*, Collider*>;
using ColliderPairSet =
    std::unordered_set<CollisionPair, Util::UnorderedPairHash>;
}  // namespace Isetta::CollisionUtil
