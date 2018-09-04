#pragma once
#include "fmod.hpp"
// TODO: do error checking later
// #include "fmod_errors.h"
#include <string>
#include <vector>

namespace Isetta {

class AudioSource {
 public:
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

class AudioSystem {
 public:
  static void Init(const std::string& filesRoot);
  static void Update();
  static void ShutDown();

  static AudioSource* LoadSound(const std::string& soundName);
  static FMOD::Channel* PlayFMODSound(FMOD::Sound* sound, bool loop,
                                      float volume);
  static void AddAudioSource(AudioSource* audioSource);
  static std::string GetMemoryReport();

 private:
  static FMOD::System* fmodSystem;
  static std::string soundFilesRoot;
  static std::vector<AudioSource*> audioSources;
};
}  // namespace Isetta