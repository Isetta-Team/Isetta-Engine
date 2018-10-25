/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
CREATE_COMPONENT_BEGIN(FrameReporter, Component)
public:
void GuiUpdate() override;

private:
int reportInterval{20};
int count{0};
float fps{};
float frameTime{};
int totalFrameCount{0};

CREATE_COMPONENT_END(FrameReporter, Component)
}  // namespace Isetta
