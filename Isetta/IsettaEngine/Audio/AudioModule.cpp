/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio/AudioModule.h"

#include <combaseapi.h>
#include <fmod_errors.h>
#include <algorithm>
#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"
#include "Core/Config/Config.h"
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Logger.h"
#include "SID/sid.h"
#include "Util.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

FMOD_RESULT F_CALLBACK LogAudioModule(FMOD_DEBUG_FLAGS flags, const char* file,
                                      int line, const char* func,
                                      const char* message) {
  char msg[1024];
  strcpy_s(msg, 1024, message);
  std::remove(std::begin(msg), std::end(msg), '\n');
  // LOG_INFO(Debug::Channel::Sound, msg);
  return FMOD_OK;
}

void AudioModule::StartUp() {
  CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
  FMOD::Memory_Initialize(
      MemoryManager::AllocOnStack(CONFIG_VAL(audioConfig.memorySize)),
      CONFIG_VAL(audioConfig.memorySize), nullptr, nullptr, nullptr);
  fmodSystem = nullptr;

  FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_LOG, FMOD_DEBUG_MODE_CALLBACK,
                         LogAudioModule);
  FMOD::System_Create(&fmodSystem);
  CheckStatus(fmodSystem->init(512, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED,
                               nullptr));
  // fmodSystem->set3DSettings(1.f, 1.f, 1.f);

  LoadAllAudioClips();
  AudioSource::audioModule = this;
  AudioListener::audioModule = this;
}

void AudioModule::Update(float deltaTime) const {
  BROFILER_CATEGORY("Audio Update", Profiler::Color::Maroon);

  if (listeners.empty()) return;
  const AudioListener* listener = *listeners.begin();
  const Math::Vector3 position = listener->transform->GetWorldPos();
  const Math::Vector3 forward = listener->transform->GetForward();
  const Math::Vector3 up = listener->transform->GetUp();

  const FMOD_VECTOR fmodPosition{position.x, position.y, position.z};
  const FMOD_VECTOR fmodForward{forward.x, forward.y, forward.z};
  const FMOD_VECTOR fmodUp{up.x, up.y, up.z};
  fmodSystem->set3DListenerAttributes(0, &fmodPosition, nullptr, &fmodForward,
                                      &fmodUp);
  fmodSystem->update();
}

void AudioModule::ShutDown() {
  for (const auto& it : soundMap) {
    CheckStatus(it.second->release());
  }

  soundMap.clear();
  CheckStatus(fmodSystem->release());
  CoUninitialize();
}

FMOD::Sound* AudioModule::FindSound(const char* soundName) {
  PROFILE
  const auto strId = SID(soundName);
  if (soundMap.find(strId) != soundMap.end()) {
    return soundMap[strId];
  }

  throw std::exception{Util::StrFormat(
      "AudioModule::FindSound => Sound file %s not found!", soundName)};
}

FMOD::Channel* AudioModule::Play(FMOD::Sound* sound, const bool loop,
                                 const float volume) const {
  FMOD::Channel* channel = nullptr;
  sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
  CheckStatus(fmodSystem->playSound(sound, nullptr, false, &channel));
  CheckStatus(channel->setVolume(volume));
  return channel;
}

void AudioModule::LoadAllAudioClips() {
  PROFILE
  std::string clipNames = CONFIG_VAL(audioConfig.audioClips);
  Util::StrRemoveSpaces(&clipNames);
  Array<std::string> clips;
  if (!clipNames.empty()) clips = Util::StrSplit(clipNames, ',');

  std::string& resourcePath = CONFIG_VAL(resourcePath) + R"(\)";
  for (const auto& file : clips) {
    FMOD::Sound* sound = nullptr;
    std::string path = resourcePath + file.data();
    CheckStatus(
        fmodSystem->createSound(path.c_str(), FMOD_LOWMEM, nullptr, &sound));

    soundMap.insert({SID(file.data()), sound});
  }
}

void PrintAudioMemoryUsage() {
  int currentAllocated;
  int maxAllocated;
  FMOD::Memory_GetStats(&currentAllocated, &maxAllocated);

  LOG_INFO(Debug::Channel::Sound, "CurMem: %d MB, MaxMem %d MB",
           Util::MegaBytesFromBytes(currentAllocated),
           Util::MegaBytesFromBytes(maxAllocated));
}

void AudioModule::CheckStatus(const FMOD_RESULT status) {
  if (status != FMOD_OK) {
    LOG_ERROR(Debug::Channel::Sound, FMOD_ErrorString(status));
    throw std::exception{Util::StrFormat("AudioSource::CheckStatus => %s",
                                         FMOD_ErrorString(status))};
  }
}
}  // namespace Isetta
