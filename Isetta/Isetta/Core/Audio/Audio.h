/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "fmod.hpp"
// TODO: do error checking
// #include "fmod_errors.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "Core/IModule.h"
#include "Core/ModuleManager.h"

namespace Isetta {
class AudioModule;

// TODO: add 3d support
class AudioSource {
 public:
  AudioSource() = default;
  ~AudioSource() = default;

  void SetAudioClip(const char* soundName);
  void Play(bool loop, float volume);
  void Pause() const;
  void Continue() const;
  void Stop() const;
  void SetVolume(const float) const;

 private:
  FMOD::Sound* fmodSound{};
  FMOD::Channel* fmodChannel{};
  static AudioModule* audioSystem;

  bool isChannelValid() const;
  friend class AudioModule;
};

class AudioModule {
 public:
  AudioModule() = default;

private:
  ~AudioModule() = default;

  void StartUp();
  void LoadAllAudioClips();
  void Update();
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
