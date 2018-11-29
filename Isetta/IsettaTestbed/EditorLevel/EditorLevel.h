/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"

/**
 * @brief Level showing the editor components: inspector, hierarchy, and console
 *
 */
namespace Isetta {
CREATE_LEVEL(EditorLevel)
void Load() override;
CREATE_LEVEL_END
}  // namespace Isetta
