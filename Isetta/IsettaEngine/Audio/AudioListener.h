/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(AudioListener, Component, true)
private:
inline static class AudioModule* audioModule;
friend class AudioModule;

public:
void OnEnable() override;
void OnDisable() override;
END_COMPONENT(AudioListener, Component);
}  // namespace Isetta
