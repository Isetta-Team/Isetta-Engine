/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

CREATE_LEVEL(EventLevel)
void OnLevelLoad() override;
CREATE_LEVEL_END