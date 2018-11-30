/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <queue>
#include "RandomMover.h"

namespace Isetta {
DEFINE_LEVEL(BVHLevel)
void Load() override;

private:
std::queue<Entity*> spheres;
Array<RandomMover*> randomMovers;
int count = 0;
DEFINE_LEVEL_END
}  // namespace Isetta
