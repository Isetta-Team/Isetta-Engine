/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include <unordered_map>
#include "fmod.hpp"
// TODO(YIDI): do error checking
// #include "fmod_errors.h"
#include "Core/IsettaAlias.h"
#include "Core/ModuleManager.h"

namespace Isetta {

class AudioModule {
 private:
  AudioModule() = default;
  ~AudioModule() = default;

  /**
   * \brief Start up the Audio module. Initialize the FMOD::System and read all
   * sound files
   */
  void StartUp();

  /**
   * \brief Read all sound files as specified in the config file into memory,
   * and store them into a unordered_map for later retrieval
   */
  void LoadAllAudioClips();

  /**
   * \brief Update the Audio module by ticking FMOD::System
   * \param deltaTime 
   */
  void Update(float deltaTime) const;

  /**
   * \brief Shut down the Audio module. Stop all sound and free all memory
   */
  void ShutDown();

  /**
   * \brief Get a handle to the sound
   * \param soundName name of the sound file you want to find
   * \return 
   */
  FMOD::Sound* FindSound(const char* soundName);

  /**
   * \brief Play the FMOD::Sound with the specified properties
   * \param sound 
   * \param loop 
   * \param volume 
   * \return 
   */
  FMOD::Channel* Play(FMOD::Sound* sound, bool loop, float volume) const;

  FMOD::System* fmodSystem;
  std::string soundFilesRoot;
  std::unordered_map<U64, FMOD::Sound*> soundMap;

  friend class AudioSource;
  friend class ModuleManager;
};

}  // namespace Isetta
