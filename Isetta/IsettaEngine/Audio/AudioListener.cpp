/*
 * Copyright (c) 2018 Isetta
 */
#include "AudioListener.h"

#include "AudioModule.h"
#include "Core/Debug/DebugDraw.h"
#include "Scene/Transform.h"

namespace Isetta {
void AudioListener::OnEnable() { audioModule->listeners.push_back(this); }
void AudioListener::OnDisable() { audioModule->listeners.remove(this); }
}  // namespace Isetta
