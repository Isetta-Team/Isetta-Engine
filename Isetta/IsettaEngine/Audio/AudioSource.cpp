/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio/AudioSource.h"

#include "AudioModule.h"
#include "Scene/Transform.h"

namespace Isetta {
AudioModule* AudioSource::audioModule;

AudioSource::AudioSource() : volume{1.f}, properties{0b100} {}

AudioSource::AudioSource(std::string_view audioClip)
    : volume{1.f}, audioClip{audioClip}, properties{0b100} {
  if (!audioClip.empty()) fmodSound = audioModule->FindSound(audioClip.data());
}

AudioSource::AudioSource(const std::bitset<3>& properties, float volume,
                         std::string_view audioClip)
    : properties{properties}, volume{volume}, audioClip{audioClip} {
  if (!audioClip.empty()) fmodSound = audioModule->FindSound(audioClip.data());
}

void AudioSource::Update() {
  if (fmodChannel && IsPlaying() && GetProperty(Property::IS_3D)) {
    const Math::Vector3 position = GetTransform()->GetWorldPos();
    const FMOD_VECTOR fmodPosition{position.x, position.y, position.z};
    fmodChannel->set3DAttributes(&fmodPosition, nullptr);
  }
}

void AudioSource::SetProperty(const Property prop, bool value) {
  switch (prop) {
    case Property::IS_3D:
      properties.set(static_cast<int>(Property::IS_3D), value);
      if (fmodSound != nullptr) {
        if (value)
          fmodSound->setMode(FMOD_3D);
        else
          fmodSound->setMode(FMOD_2D);
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
  };
}

void AudioSource::SetAudioClip(std::string_view audioClip) {
  this->audioClip = audioClip;
  fmodSound = audioModule->FindSound(audioClip.data());
}

void AudioSource::Play() {
  if (isSoundValid()) {
    if (GetProperty(Property::IS_3D))
      fmodSound->setMode(FMOD_3D);
    else
      fmodSound->setMode(FMOD_2D);
    fmodChannel =
        audioModule->Play(fmodSound, GetProperty(Property::LOOP), volume);
    fmodChannel->setLoopCount(loopCount);
    fmodChannel->setMute(GetProperty(Property::IS_MUTE));
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

void AudioSource::SetVolume(const float inVolume) {
  volume = inVolume;
  if (fmodChannel) AudioModule::CheckStatus(fmodChannel->setVolume(volume));
}

void AudioSource::Mute(bool mute) {
  SetProperty(Property::IS_MUTE, mute);
  if (fmodChannel) AudioModule::CheckStatus(fmodChannel->setMute(mute));
}

void AudioSource::SetSpeed(const float inSpeed) {
  speed = inSpeed;
  if (fmodSound) AudioModule::CheckStatus(fmodSound->setMusicSpeed(speed));
}
float AudioSource::GetSpeed() const { return speed; }

bool AudioSource::IsPlaying() const {
  bool isPlaying = false;
  fmodChannel->isPlaying(&isPlaying);
  return isPlaying;
}

void AudioSource::LoopFor(int count) {
  loopCount = count;
  SetProperty(Property::LOOP, count > 0);
  if (fmodSound) AudioModule::CheckStatus(fmodSound->setLoopCount(count));
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
