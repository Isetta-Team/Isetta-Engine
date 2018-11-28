/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {
CREATE_LEVEL(CollisionSolverLevel)
Entity* box = nullptr;
Entity* capsule = nullptr;
Entity* sphere = nullptr;
void Load() override;
CREATE_LEVEL_END
}  // namespace Isetta
