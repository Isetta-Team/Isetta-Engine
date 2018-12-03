/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(AudioListener, Component, true)
private:
inline static class AudioModule* audioModule;
friend class AudioModule;

public:
void OnEnable() override;
void OnDisable() override;
DEFINE_COMPONENT_END(AudioListener, Component);
}  // namespace Isetta
