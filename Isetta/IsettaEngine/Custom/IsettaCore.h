// collection of headers that are used very frequently

#pragma once
#include "Application.h"
#include "Audio/AudioSource.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Debug/Logger.h"
#include "Core/Time/Clock.h"
#include "Core/Time/Time.h"
#include "EngineLoop.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/GUI.h"
#include "Graphics/LightComponent.h"
#include "Graphics/RectTransform.h"
#include "Input/Input.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"
#include "Scene/Transform.h"
#include "Util.h"
#define ADD_ENTITY(name) LevelManager::Instance().currentLevel->AddEntity(name)
