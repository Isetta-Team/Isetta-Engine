/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

using namespace Isetta;

CREATE_LEVEL(GUILevel)
void OnLevelLoad() override;
CREATE_LEVEL_END
