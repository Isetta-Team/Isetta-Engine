/*
 * Copyright (c) 2018 Isetta
 */
#include "AudioSource.h"

#include "AudioClip.h"
#include "AudioModule.h"
#include "FMOD/inc/fmod.hpp"
#include "Scene/Transform.h"

namespace Isetta {
AudioModule* AudioSource::audioModule;

AudioSource::AudioSource(AudioClip* clip) : clip{clip} {}

AudioSource::AudioSource(const std::bitset<3>& properties, AudioClip* clip)
    : clip{clip}, properties{properties} {}

void AudioSource::Update() {
  if (IsChannelValid() && IsPlaying() && GetProperty(Property::IS_3D)) {
    const Math::Vector3 position = transform->GetWorldPos();
    const FMOD_VECTOR fmodPosition{position.x, position.y, position.z};
    fmodChannel->set3DAttributes(&fmodPosition, nullptr);
  }
}

void AudioSource::SetProperty(const Property prop, const bool value) {
  switch (prop) {
    case Property::IS_3D:
      properties.set(static_cast<int>(Property::IS_3D), value);
      if (clip) {
        clip->fmodSound->setMode(value ? FMOD_3D : FMOD_2D);
      }
      break;
    case Property::LOOP:
      properties.set(static_cast<int>(Property::LOOP), value);
      break;
    case Property::IS_MUTE:
      properties.set(static_cast<int>(Property::IS_MUTE), value);
      if (fmodChannel) AudioModule::CheckStatus(fmodChannel->setMute(value));
      break;
  };
}

bool AudioSource::GetProperty(const Property prop) const {
  switch (prop) {
    case Property::IS_3D:
      return properties.test(static_cast<int>(Property::IS_3D));
    case Property::LOOP:
      return properties.test(static_cast<int>(Property::LOOP));
    case Property::IS_MUTE:
      return properties.test(static_cast<int>(Property::IS_MUTE));
  }
  return false;
}

void AudioSource::Play() {
  if (IsSoundValid()) {
    audioModule->Play(this);
  }
}

void AudioSource::Pause() const {
  if (IsChannelValid()) {
    AudioModule::CheckStatus(fmodChannel->setPaused(true));
  }
}

void AudioSource::Continue() const {
  if (IsChannelValid()) {
    AudioModule::CheckStatus(fmodChannel->setPaused(false));
  }
}

void AudioSource::Stop() const {
  if (IsChannelValid() && IsPlaying()) {
    AudioModule::CheckStatus(fmodChannel->stop());
  }
}

void AudioSource::SetVolume(const float inVolume) {
  volume = Math::Util::Clamp01(inVolume);
  if (IsChannelValid()) {
    AudioModule::CheckStatus(fmodChannel->setVolume(volume));
  }
}

float AudioSource::GetVolume() const { return volume; }

void AudioSource::SetMinMaxDistance(Math::Vector2 minMax) {
  minMaxDistance = minMax;
  if (IsChannelValid() && GetProperty(Property::IS_3D)) {
    AudioModule::CheckStatus(
        fmodChannel->set3DMinMaxDistance(minMax.x, minMax.y));
  }
}

Math::Vector2 AudioSource::GetMinMaxDistance() const { return minMaxDistance; }

// void AudioSource::SetSpeed(const float inSpeed) {
//  speed = inSpeed;
//  if (clip) AudioModule::CheckStatus(clip->fmodSound->setMusicSpeed(speed));
//}
// float AudioSource::GetSpeed() const { return speed; }

bool AudioSource::IsPlaying() const {
  if (IsChannelValid()) {
    bool isPlaying{false}, isPaused{false};
    AudioModule::CheckStatus(fmodChannel->isPlaying(&isPlaying));
    AudioModule::CheckStatus(fmodChannel->getPaused(&isPaused));
    return isPlaying && !isPaused;
  }
  return false;
}

bool AudioSource::IsPaused() const {
  if (IsChannelValid()) {
    bool isPaused;
    AudioModule::CheckStatus(fmodChannel->getPaused(&isPaused));
    return isPaused;
  }
  return false;
}

bool AudioSource::IsStarted() const {
  if (IsChannelValid()) {
    bool isPlaying;
    AudioModule::CheckStatus(fmodChannel->isPlaying(&isPlaying));
    return isPlaying;
  }
  return false;
}

void AudioSource::LoopFor(const int count) {
  loopCount = count;
  SetProperty(Property::LOOP, count > 0);
}

bool AudioSource::IsChannelValid() const {
  if (!fmodChannel) return false;
  int index;
  FMOD_RESULT res = fmodChannel->getIndex(&index);
  if (res == FMOD_ERR_INVALID_HANDLE || res == FMOD_ERR_CHANNEL_STOLEN) {
    return false;
  }
  return true;
}

bool AudioSource::IsSoundValid() const {
  if (!clip) {
    throw std::exception{
        "AudioSource::isSoundValid => Audio clip for this audio source is not "
        "found!"};
  }

  return true;
}
}  // namespace Isetta
