/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/IsettaLevel.h"

/**
 * @brief Level showing the editor components: inspector, heirarchy, and console
 *
 */
namespace Isetta {
CREATE_LEVEL(EditorLevel)
void OnLevelLoad() override;
CREATE_LEVEL_END
}  // namespace Isetta
