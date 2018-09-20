/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio/AudioModule.h"
#include <combaseapi.h>
#include "Audio/AudioSource.h"
#include "Core/Debug/Logger.h"
#include "SID/sid.h"

namespace Isetta {

void AudioModule::StartUp() {
  CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  // FMOD::Memory_Initialize(std::malloc(10_MB), 10_MB, nullptr, nullptr,
  // nullptr);
  fmodSystem = nullptr;
  FMOD::System_Create(&fmodSystem);
  fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);
  // TODO(YIDI): Set this in engine config
  soundFilesRoot = R"(Resources\Sound\)";
  LoadAllAudioClips();
  AudioSource::audioSystem = this;
}

void AudioModule::Update(float deltaTime) const { fmodSystem->update(); }

void AudioModule::ShutDown() {
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

  throw std::exception{std::string("AudioModule::FindSound => Sound file " +
                                   std::string(soundName) + " found!")
                           .c_str()};
}

FMOD::Channel* AudioModule::Play(FMOD::Sound* sound, const bool loop,
                                 const float volume) const {
  FMOD::Channel* channel = nullptr;
  sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
  fmodSystem->playSound(sound, nullptr, false, &channel);
  channel->setVolume(volume);
  return channel;
}

void AudioModule::LoadAllAudioClips() {
  // TODO(YIDI): get this array of string from game config
  const char* files[]{"singing.wav", "wave.mp3", "gunshot.aiff"};

  for (auto file : files) {
    StringId hashedId = SID(file);

    FMOD::Sound* sound = nullptr;
    std::string path = soundFilesRoot + file;
    fmodSystem->createSound(path.c_str(), FMOD_LOWMEM, nullptr, &sound);

    soundMap.insert({hashedId.GetValue(), sound});
  }
}

inline float MegaBytesFromBytes(const int byte) {
  return byte / 1024.f / 1024.f;
}

void PrintAudioMemoryUsage() {
  int currentAllocated;
  int maxAllocated;
  FMOD::Memory_GetStats(&currentAllocated, &maxAllocated);

  LOG_INFO(Debug::Channel::Sound, "CurMem: %d MB, MaxMem %d MB",
           MegaBytesFromBytes(currentAllocated),
           MegaBytesFromBytes(maxAllocated));
}
}  // namespace Isetta
