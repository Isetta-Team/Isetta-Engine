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

namespace Isetta {

class AudioModule {
 public:
  AudioModule() = default;

 private:
  ~AudioModule() = default;

  void StartUp();
  void LoadAllAudioClips();
  void Update(float deltaTime);
  void ShutDown();

  FMOD::Sound* FindSound(const char* soundName);
  FMOD::Channel* Play(FMOD::Sound* sound, bool loop, float volume) const;

  FMOD::System* fmodSystem;
  std::string soundFilesRoot;
  std::unordered_map<std::uint64_t, FMOD::Sound*> soundMap;

  friend class AudioSource;
  friend class ModuleManager;
};

}  // namespace Isetta
