/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"

/**
 * @brief Demonstrating level loading
 *
 */
namespace Isetta {
CREATE_LEVEL(LevelLoadingLevel)
void Load() override;
CREATE_LEVEL_END
}  // namespace Isetta
