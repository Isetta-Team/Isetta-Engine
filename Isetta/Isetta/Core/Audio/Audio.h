/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "fmod.hpp"
// TODO: do error checking
// #include "fmod_errors.h"
#include <string>
#include <vector>
#include "Core/IModule.h"

namespace Isetta {

// TODO: add 3d support
class AudioSource {
 public:
  static AudioSource* LoadSound(const std::string& soundName);

  explicit AudioSource(FMOD::Sound*);
  void Play(bool loop, float volume);
  // void Play3D(Vector3, bool loop, float volume);
  void Pause() const;
  void Continue() const;
  void Stop() const;
  void SetVolume(const float) const;
  void Erase();

  bool isErased;

 private:
  FMOD::Sound* fmodSound;
  FMOD::Channel* fmodChannel{};

  bool isChannelValid() const;
};

class AudioSystem : IModule {
 public:
  AudioSystem() {}
  ~AudioSystem() final {}

  void StartUp() final;
  void Update() const;
  void ShutDown() final;

  static std::string GetMemoryReport();

 private:
  AudioSource* LoadSound(const std::string& soundName) const;
  FMOD::Channel* PlayFMODSound(FMOD::Sound* sound, bool loop,
                               float volume) const;
  void AddAudioSource(AudioSource* audioSource);

  FMOD::System* fmodSystem;
  std::string soundFilesRoot;
  std::vector<AudioSource*> audioSources;

  friend class AudioSource;
};

extern AudioSystem gAudioSystem;
}  // namespace Isetta
