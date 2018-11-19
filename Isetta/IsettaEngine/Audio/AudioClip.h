/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include <unordered_map>
#include "ISETTA_API.h"

namespace FMOD {
class Sound;
}
class StringId;

namespace Isetta {
class ISETTA_API AudioClip {
 private:
  std::string filePath, name;
  FMOD::Sound* fmodSound;

  static class AudioModule* audioModule;
  friend class AudioModule;
  friend class AudioSource;

  static std::unordered_map<StringId, AudioClip*> clips;

  static void LoadConfigClips();
  static void UnloadAll();

 public:
  AudioClip(const std::string_view filePath, const std::string_view name = "");
  ~AudioClip();

  // void Unload() const;
  static AudioClip* GetClip(const std::string_view name);
};
}  // namespace Isetta