/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <queue>
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace Isetta {
/**
 * @brief FrameReporter reports FPS, frame time, last frame time, and average
 * frame time
 *
 */
DEFINE_COMPONENT(FrameReporter, Component, false)
public:
void GuiUpdate() override;
void Open();

private:
int reportInterval{20};
int count{0};
float fps{};
float frameTime{};

float timeSumForAvg{0.f};
Size frameCountForAvg{60};
std::queue<float> frameDurations;

bool isOpen{false};
DEFINE_COMPONENT_END(FrameReporter, Component)
}  // namespace Isetta
