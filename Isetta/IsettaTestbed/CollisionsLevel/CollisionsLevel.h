/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

/**
 * @brief Level testing our collision intersections
 *
 */
namespace Isetta {
CREATE_LEVEL(CollisionsLevel)
void Load() override;
CREATE_LEVEL_END
}  // namespace Isetta
