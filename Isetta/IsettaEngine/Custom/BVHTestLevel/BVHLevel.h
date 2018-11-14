/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <queue>
#include "Scene/IsettaLevel.h"
#include "Core/DataStructures/Array.h"
#include "Custom/BVHTestLevel/RandomMover.h"

namespace Isetta {
CREATE_LEVEL(BVHLevel)
void LoadLevel() override;

private:
std::queue<Entity*> spheres;
Array<RandomMover*> randomMovers;
int count = 0;
CREATE_LEVEL_END
}  // namespace Isetta
