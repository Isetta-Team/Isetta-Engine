// collection of headers that are used very frequently

#pragma once
#include "Audio/AudioSource.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Debug/Logger.h"
#include "Core/Time/Clock.h"
#include "Core/Time/Time.h"
#include "EngineLoop.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Input/Input.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Util.h"
#include "Scene/LevelManager.h"
#include "Scene/Level.h"
#define ADD_ENTITY(name) LevelManager::Instance().currentLevel->AddEntity(name)
