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
DEFINE_LEVEL(EmptyLevel)
void Load() override;
DEFINE_LEVEL_END
}  // namespace Isetta
