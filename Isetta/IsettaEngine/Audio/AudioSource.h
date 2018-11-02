/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"
#include "fmod.hpp"

namespace Isetta {
class AudioModule;

// TODO(YIDI): add 3d support
/**
 * \brief AudioSource is the class used to play specific sound clips
 */
BEGIN_COMPONENT(AudioSource, Component, false)
public:
/**
 * \brief Set the audio clip to be played on this AudioSource
 * \param soundName
 */
void SetAudioClip(const char* soundName);

/**
 * \brief Play the sound with specified properties
 * \param loop
 * \param volume
 */
void Play(bool loop, float volume);

/**
 * \brief Pause the sound, preserving progress
 */
void Pause() const;

/**
 * \brief Continue a paused sound
 */
void Continue() const;

/**
 * \brief Stop the sound completely
 */
void Stop() const;

/**
 * \brief Set volume of the sound being played
 */
void SetVolume(float) const;

/**
 * \brief Change speed of music, only supports MOD/S3M/XM/IT/MIDI file formats
 */
void SetSpeed(float) const;

/**
 * \brief Check if the sound is currently playing
 * \return
 */
bool IsPlaying() const;

private:
FMOD::Sound* fmodSound{};
FMOD::Channel* fmodChannel{};
static AudioModule* audioModule;

bool isChannelValid() const;
bool isSoundValid() const;
friend class AudioModule;
END_COMPONENT(AudioSource, Component);
}  // namespace Isetta
