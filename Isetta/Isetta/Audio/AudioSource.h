/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "fmod.hpp"

namespace Isetta {
class AudioModule;

// TODO(YIDI): add 3d support
class AudioSource {
 public:
  AudioSource() = default;
  ~AudioSource() = default;

  void SetAudioClip(const char* soundName);
  void Play(bool loop, float volume);
  void Pause() const;
  void Continue() const;
  void Stop() const;
  void SetVolume(float) const;

 private:
  FMOD::Sound* fmodSound{};
  FMOD::Channel* fmodChannel{};
  static AudioModule* audioSystem;

  bool isChannelValid() const;
  friend class AudioModule;
};

}  // namespace Isetta
