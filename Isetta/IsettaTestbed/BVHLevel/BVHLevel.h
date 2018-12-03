/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <queue>

/**
 * @brief Level testing our dynamic AABB tree
 *
 */
namespace Isetta {
DEFINE_LEVEL(BVHLevel)
void Load() override;

private:
std::queue<Entity*> spheres;
Array<class RandomMover*> randomMovers;
int count = 0;
DEFINE_LEVEL_END
}  // namespace Isetta
