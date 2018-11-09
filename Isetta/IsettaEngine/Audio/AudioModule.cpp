/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio/AudioModule.h"
#include <SID/sid.h>
#include <combaseapi.h>
#include <fmod_errors.h>
#include <algorithm>
#include "Audio/AudioSource.h"
#include "Core/Config/Config.h"
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Logger.h"
#include "Util.h"

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
  CheckStatus(fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr));

  auto& config = Config::Instance();
  soundFilesRoot = config.resourcePath.GetVal() + R"(\)" +
                   config.audioConfig.pathUnderResource.GetVal() + R"(\)";
  LoadAllAudioClips();
  AudioSource::audioModule = this;
}

void AudioModule::Update(float deltaTime) const { fmodSystem->update(); }

void AudioModule::ShutDown() {
  for (const auto& it : soundMap) {
    CheckStatus(it.second->release());
  }

  soundMap.clear();
  CheckStatus(fmodSystem->release());
  CoUninitialize();
}

FMOD::Sound* AudioModule::FindSound(const char* soundName) {
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
  std::string clipNames = CONFIG_VAL(audioConfig.audioClips);
  Util::StrRemoveSpaces(&clipNames);
  Array<std::string_view>& clips = Util::StrSplit(clipNames, ',');

  for (const auto& file : clips) {
    FMOD::Sound* sound = nullptr;
    std::string path = soundFilesRoot + file.data();
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
