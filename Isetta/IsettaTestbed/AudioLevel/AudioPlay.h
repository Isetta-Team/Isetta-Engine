/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Input/KeyCode.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(AudioPlay, Component, true)
private:
class AudioSource* const src = nullptr;
KeyCode key;
int handle;

public:
AudioPlay(KeyCode key, class AudioSource* const src) : key{key}, src{src} {}

void OnEnable() override;
void OnDisable() override;
END_COMPONENT(AudioPlay, Component)
}  // namespace Isetta
