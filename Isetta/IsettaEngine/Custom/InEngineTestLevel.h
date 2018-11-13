/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"
#include <queue>

namespace Isetta {
CREATE_LEVEL(InEngineTestLevel)
void LoadLevel() override;
std::queue<Entity*> zombies;
CREATE_LEVEL_END
}  // namespace Isetta
