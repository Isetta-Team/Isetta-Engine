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
void OnLevelLoad() override;
CREATE_LEVEL_END
}  // namespace Isetta
