/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <queue>
#include "Core/DataStructures/Array.h"
#include "RandomMover.h"
#include "Scene/IsettaLevel.h"

namespace Isetta {
CREATE_LEVEL(BVHLevel)
void OnLevelLoad() override;

private:
std::queue<Entity*> spheres;
Array<RandomMover*> randomMovers;
int count = 0;
CREATE_LEVEL_END
}  // namespace Isetta
