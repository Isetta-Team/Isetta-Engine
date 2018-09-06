/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio.h"

#include "combaseapi.h"

#include <sid/sid.h>
#include <iomanip>
#include <sstream>

namespace Isetta {

AudioModule* AudioSource::audioSystem;

AudioSource::AudioSource() {
  isDeleted = false;
  audioSystem->AddAudioSource(this);
}

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

void AudioModule::StartUp() {
  fmodSystem = nullptr;
  FMOD::System_Create(&fmodSystem);
  fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);
  // TODO: Set this in engine config
  soundFilesRoot = R"(Resources\Sound\)";
  LoadAllAudioClips();
  AudioSource::audioSystem = this;
}

void AudioModule::Update() { fmodSystem->update(); }

void AudioModule::ShutDown() {
  for (auto it : audioSources) {
    if (!it->isDeleted) {
      delete (it);
    }
  }

  audioSources.clear();
  for (auto it : soundMap) {
    it.second->release();
  }

  soundMap.clear();
  fmodSystem->release();
  CoUninitialize();
}

FMOD::Sound* AudioModule::FindSound(const char* soundName) {
  const auto hashedValue = SID(soundName).GetValue();
  if (soundMap.find(hashedValue) != soundMap.end()) {
    return soundMap[hashedValue];
  }
  return nullptr;
}

FMOD::Channel* AudioModule::Play(FMOD::Sound* sound, bool loop,
                                 float volume) const {
  FMOD::Channel* channel = nullptr;
  sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
  fmodSystem->playSound(sound, nullptr, false, &channel);
  channel->setVolume(volume);
  return channel;
}

void AudioModule::LoadAllAudioClips() {
  // TODO: get this array of string from game config
  const char* files[]{"singing.wav", "wave.mp3"};

  for (auto file : files) {
    StringId hashedId = SID(file);

    FMOD::Sound* sound = nullptr;
    std::string path = soundFilesRoot + file;
    fmodSystem->createSound(path.c_str(), FMOD_LOWMEM, nullptr, &sound);

    soundMap.insert({hashedId.GetValue(), sound});
  }
}

void AudioModule::AddAudioSource(AudioSource* audioSource) {
  audioSources.push_back(audioSource);
}

inline float MegaBytesFromBytes(const int byte) {
  return byte / 1024.f / 1024.f;
}

std::string GetAudioSystemMemoryReport() {
  int currentAllocated;
  int maxAllocated;
  FMOD::Memory_GetStats(&currentAllocated, &maxAllocated);

  std::stringstream memReport;
  memReport << std::fixed << std::setprecision(2)
            << "------------- Audio Engine Memory Usage ------------------"
            << std::endl
            << "Current usage: " << MegaBytesFromBytes(currentAllocated)
            << "mb, Max usage: " << MegaBytesFromBytes(maxAllocated) << "mb.\n"
            << std::endl;

  return memReport.str();
}
}  // namespace Isetta
