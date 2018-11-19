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
    else if (src->IsPlaying())
      src->Pause();
    else
      src->Play();
  });
}
void AudioPlay::OnDisable() { Input::UnregisterKeyPressCallback(key, handle); }
}  // namespace Isetta