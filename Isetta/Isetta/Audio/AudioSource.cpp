/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio/AudioSource.h"
#include "Audio/AudioModule.h"
#include "Core/Debug/Logger.h"

namespace Isetta {

AudioModule* AudioSource::audioSystem;

void AudioSource::SetAudioClip(const char* soundName) {
  fmodSound = audioSystem->FindSound(soundName);
}

void AudioSource::Play(const bool loop, const float volume) {
  if (isSoundValid()) {
    fmodChannel = audioSystem->Play(fmodSound, loop, volume);
  }
}

void AudioSource::Pause() const {
  if (isChannelValid() && IsPlaying()) {
    AudioModule::CheckStatus(fmodChannel->setPaused(true));
  }
}

void AudioSource::Continue() const {
  if (isChannelValid()) {
    AudioModule::CheckStatus(fmodChannel->setPaused(false));
  }
}

void AudioSource::Stop() const {
  if (isChannelValid() && IsPlaying()) {
    AudioModule::CheckStatus(fmodChannel->stop());
  }
}

void AudioSource::SetVolume(const float volume) const {
  if (isChannelValid()) {
    AudioModule::CheckStatus(fmodChannel->setVolume(volume));
  }
}

void AudioSource::SetSpeed(const float speed) const {
  if (isSoundValid()) {
    AudioModule::CheckStatus(fmodSound->setMusicSpeed(speed));
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

bool AudioSource::isSoundValid() const {
  if (fmodSound == nullptr) {
    throw std::exception{
        "AudioSource::isSoundValid => Sound clip for this audio source is not "
        "found!"};
  }

  return true;
}

}  // namespace Isetta
