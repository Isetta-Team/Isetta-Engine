/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
namespace Isetta {
DEFINE_COMPONENT(AudioPlay, Component, false)
private:
class AudioSource* const src = nullptr;
KeyCode key;
U64 handle;

public:
AudioPlay(KeyCode key, class AudioSource* const src) : key{key}, src{src} {}

void OnEnable() override;
void OnDisable() override;
DEFINE_COMPONENT_END(AudioPlay, Component)
}  // namespace Isetta
