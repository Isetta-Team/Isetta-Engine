/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio/AudioModule.h"
#include "Audio/AudioSource.h"

namespace Isetta {

AudioModule* AudioSource::audioSystem;

void AudioSource::SetAudioClip(const char* soundName) {
  fmodSound = audioSystem->FindSound(soundName);
}

void AudioSource::Play(const bool loop, const float volume) {
  if (fmodSound != nullptr) {
    fmodChannel = audioSystem->Play(fmodSound, loop, volume);
  }
}

void AudioSource::Pause() const {
  if (isChannelValid()) {
    fmodChannel->setPaused(true);
  }
}

void AudioSource::Continue() const {
  if (isChannelValid()) {
    fmodChannel->setPaused(false);
  }
}

void AudioSource::Stop() const {
  if (isChannelValid()) {
    fmodChannel->stop();
  }
}

void AudioSource::SetVolume(const float volume) const {
  if (isChannelValid()) {
    fmodChannel->setVolume(volume);
  }
}

bool AudioSource::isChannelValid() const {
  bool isPlaying = false;
  fmodChannel->isPlaying(&isPlaying);
  return fmodChannel != nullptr && isPlaying;
}
}  // namespace Isetta
