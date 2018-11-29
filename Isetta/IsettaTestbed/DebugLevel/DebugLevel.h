/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

using namespace Isetta;

/**
 * @brief Level demoing our debug drawing capabilities
 *
 */
CREATE_LEVEL(DebugLevel)
void Load() override;
CREATE_LEVEL_END
