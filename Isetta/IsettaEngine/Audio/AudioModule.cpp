/*
 * Copyright (c) 2018 Isetta
 */
#include "Audio/AudioModule.h"

#include <combaseapi.h>
#include <fmod_errors.h>
#include <algorithm>
#include "Audio/AudioClip.h"
#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
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

  AudioSource::audioModule = this;
  AudioListener::audioModule = this;
  AudioClip::audioModule = this;
  AudioClip::LoadConfigClips();
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
  AudioClip::UnloadAll();
  CheckStatus(fmodSystem->release());
  CoUninitialize();
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

void AudioModule::LoadClip(AudioClip* const clip) const {
  clip->fmodSound = nullptr;
  const std::string fullPath =
      CONFIG_VAL(resourcePath) + R"(\)" + clip->filePath;
  CheckStatus(fmodSystem->createSound(fullPath.c_str(), FMOD_LOWMEM, nullptr,
                                      &clip->fmodSound));
}

void AudioModule::Play(AudioSource* const source) const {
  using Property = AudioSource::Property;
  U64 mode = 0;
  mode |= source->GetProperty(Property::IS_3D) ? FMOD_3D : FMOD_2D;
  mode |=
      source->GetProperty(Property::LOOP) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
  CheckStatus(source->clip->fmodSound->setMode(mode));
  source->fmodChannel = nullptr;
  CheckStatus(fmodSystem->playSound(source->clip->fmodSound, nullptr, false,
                                    &source->fmodChannel));
  CheckStatus(source->fmodChannel->setVolume(source->volume));
  CheckStatus(source->fmodChannel->setLoopCount(source->loopCount));
  CheckStatus(
      source->fmodChannel->setMute(source->GetProperty(Property::IS_MUTE)));
}
}  // namespace Isetta
