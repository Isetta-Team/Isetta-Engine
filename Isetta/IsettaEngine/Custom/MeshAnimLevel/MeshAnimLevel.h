/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <queue>
#include "Scene/IsettaLevel.h"

namespace Isetta {
CREATE_LEVEL(MeshAnimLevel)
void LoadLevel() override;
std::queue<Entity*> zombies;
CREATE_LEVEL_END
}  // namespace Isetta
