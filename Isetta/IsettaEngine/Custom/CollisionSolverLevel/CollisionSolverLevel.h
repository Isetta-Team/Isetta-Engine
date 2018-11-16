/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {
CREATE_LEVEL(CollisionSolverLevel)
void LoadLevel() override;
};  // namespace Isetta
}  // namespace Isetta
