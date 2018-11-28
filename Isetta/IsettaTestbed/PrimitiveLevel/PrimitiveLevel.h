/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"

/**
 * @brief Level displaying all the types of primitive objects
 *
 */
namespace Isetta {
CREATE_LEVEL(PrimitiveLevel)
void OnLevelLoad() override;
CREATE_LEVEL_END
}  // namespace Isetta
