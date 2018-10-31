/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"
#include <queue>

namespace Isetta {
CREATE_LEVEL(BVHLevel)
void LoadLevel() override;

private:
std::queue<Entity*> spheres;
int count = 0;
};
}  // namespace Isetta
