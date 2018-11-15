/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <queue>
#include "Scene/IsettaLevel.h"

namespace Isetta {
CREATE_LEVEL(BVHLevel)
void LoadLevel() override;

private:
std::queue<Entity*> spheres;
int count = 0;
CREATE_LEVEL_END
}  // namespace Isetta
