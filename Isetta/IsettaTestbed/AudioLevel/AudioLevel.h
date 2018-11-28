/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"
#include "Scene/Level.h"

/**
 * @brief Level to demonstrate 2D and 3D audio
 *
 */
namespace Isetta {
CREATE_LEVEL(AudioLevel)
void Load() override;
CREATE_LEVEL_END
}  // namespace Isetta
