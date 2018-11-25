/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <list>
#include <string>
#include "Core/Config/CVar.h"
#include "Core/IsettaAlias.h"
#include "FMOD/inc/fmod.hpp"

namespace Isetta {
class AudioModule {
 public:
  struct AudioConfig {
    CVar<Size> memorySize{"audio_memory_size", 10485760};
  };

 private:
  AudioModule() = default;
  ~AudioModule() = default;

  /**
   * \brief Start up the Audio module. Initialize the FMOD::System and read all
   * sound files
   */
  void StartUp();

  /**
   * \brief Update the Audio module by ticking FMOD::System
   * \param deltaTime
   */
  void Update(float deltaTime) const;

  /**
   * \brief Shut down the Audio module. Stop all sound and free all memory
   */
  void ShutDown();

  void UnloadLevel();

  /**
   * \brief Check FMOD_RESULT and throw exception when there's an error
   */
  static void CheckStatus(FMOD_RESULT);

  void LoadClip(class AudioClip* const clip) const;
  /**
   * \brief Play the sound with the specified properties
   * \param audio AudioSource
   * \return
   */
  void Play(class AudioSource* const source) const;

  FMOD::System* fmodSystem;
  std::list<class AudioListener*> listeners;

  friend class AudioClip;
  friend class AudioSource;
  friend class AudioListener;
  friend class EngineLoop;
  friend class StackAllocator;
};

}  // namespace Isetta
