/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"
#include "Scene/Level.h"

namespace Isetta {
CREATE_LEVEL(AudioLevel)
void OnLevelLoad() override;
CREATE_LEVEL_END
}  // namespace Isetta
