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

  /**
   * @brief Load the AudioClip
   *
   * @param filePath under the resource_path where audio located
   * @param soundName user defined name for easy access, defaults to filepath
   * @return AudioClip* AudioClip that is loaded
   */
  static AudioClip* Load(const std::string_view filePath,
                         const std::string_view soundName = "");
  /**
   * @brief Finds the AudioClip by name
   *
   * @param name of the AudioClip (user defined name on load)
   * @return AudioClip* AudioClip associated with name or nullptr
   */
  static AudioClip* Find(const std::string_view name);

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