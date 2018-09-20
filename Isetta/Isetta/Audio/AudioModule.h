/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include <unordered_map>
#include "fmod.hpp"
// TODO(YIDI): do error checking
// #include "fmod_errors.h"
#include "Core/ModuleManager.h"
#include "Core/IsettaAlias.h"

namespace Isetta {

class AudioModule {
 private:
  AudioModule() = default;
  ~AudioModule() = default;

  void StartUp();
  void LoadAllAudioClips();
  void Update(float deltaTime) const;
  void ShutDown();

  FMOD::Sound* FindSound(const char* soundName);
  FMOD::Channel* Play(FMOD::Sound* sound, bool loop, float volume) const;

  FMOD::System* fmodSystem;
  std::string soundFilesRoot;
  std::unordered_map<U64, FMOD::Sound*> soundMap;

  friend class AudioSource;
  friend class ModuleManager;
};

}  // namespace Isetta
