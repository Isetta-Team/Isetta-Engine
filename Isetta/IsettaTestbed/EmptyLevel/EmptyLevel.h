/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"

/**
 * @brief Empty level to be used as a starting point for user created levels
 *
 */
namespace Isetta {
CREATE_LEVEL(EmptyLevel)
void OnLevelLoad() override;
CREATE_LEVEL_END
}  // namespace Isetta
