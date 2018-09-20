/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio/AudioSource.h"
#include "Audio/AudioModule.h"

namespace Isetta {

AudioModule* AudioSource::audioSystem;

void AudioSource::SetAudioClip(const char* soundName) {
  fmodSound = audioSystem->FindSound(soundName);
}

void AudioSource::Play(const bool loop, const float volume) {
  if (fmodSound != nullptr) {
    fmodChannel = audioSystem->Play(fmodSound, loop, volume);
  } else {
    throw std::exception{
        "AudioSource::Play => Sound clip for this audio source is not found!"};
  }
}

void AudioSource::Pause() const {
  if (isChannelValid() && IsPlaying()) {
    fmodChannel->setPaused(true);
  }
}

void AudioSource::Continue() const {
  if (isChannelValid()) {
    fmodChannel->setPaused(false);
  }
}

void AudioSource::Stop() const {
  if (isChannelValid() && IsPlaying()) {
    fmodChannel->stop();
  }
}

void AudioSource::SetVolume(const float volume) const {
  if (isChannelValid()) {
    fmodChannel->setVolume(volume);
  }
}

bool AudioSource::IsPlaying() const {
  bool isPlaying = false;
  fmodChannel->isPlaying(&isPlaying);
  return isPlaying;
}

bool AudioSource::isChannelValid() const {
  if (fmodChannel == nullptr) {
    throw std::exception{
        "AudioSource::isChannelValid => There is no sound playing on this "
        "AudioSource"};
  }

  return true;
}
}  // namespace Isetta
