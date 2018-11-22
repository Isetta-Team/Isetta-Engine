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
 public:
  ~AudioClip();

  // soundName defaults to filePath if empty
  static AudioClip* LoadClip(std::string_view filePath,
                             std::string_view soundName = "");
  static AudioClip* GetClip(std::string_view name);

 private:
  explicit AudioClip(std::string_view filePath, std::string_view name);

  static void UnloadAll();

  std::string filePath, name;
  FMOD::Sound* fmodSound{};

  static class AudioModule* audioModule;
  static std::unordered_map<StringId, AudioClip*> clips;

  friend class AudioModule;
  friend class AudioSource;
  friend class FreeListAllocator;
};
}  // namespace Isetta