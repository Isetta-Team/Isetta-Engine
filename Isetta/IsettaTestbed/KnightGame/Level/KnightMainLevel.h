/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"
#include "Scene/Level.h"

namespace KnightGame {
CREATE_LEVEL(KnightMainLevel)
void OnLevelLoad() override;
CREATE_LEVEL_END
}  // namespace KnightGame
