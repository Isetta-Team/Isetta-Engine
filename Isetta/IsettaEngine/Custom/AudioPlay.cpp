/*
 * Copyright (c) 2018 Isetta
 */
#include "AudioPlay.h"

#include "Audio/AudioSource.h"
#include "Input/Input.h"

namespace Isetta {
void AudioPlay::OnEnable() {
  src->Play();
  src->Pause();
  handle = Input::RegisterKeyPressCallback(key, [&]() {
    if (src->IsPaused())
      src->Continue();
    else
      src->Pause();
  });
}
void AudioPlay::OnDisable() { Input::UnregisterKeyPressCallback(key, handle); }
}  // namespace Isetta