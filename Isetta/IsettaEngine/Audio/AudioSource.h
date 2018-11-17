/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "FMOD/inc/fmod.hpp"
#include "Scene/Component.h"

namespace Isetta {
class AudioModule;

// TODO(YIDI): add 3d support
/**
 * \brief AudioSource is the class used to play specific sound clips
 */
BEGIN_COMPONENT(AudioSource, Component, false)
public:
enum class Property {
  IS_3D,
  LOOP,
  IS_MUTE,
};

AudioSource();
explicit AudioSource(std::string_view audioClip);
AudioSource(const std::bitset<3>& properties, float volume,
            std::string_view audioClip = "");

void Update() override;

void SetProperty(const Property prop, bool value);
bool GetProperty(const Property prop) const;

/**
 * \brief Set the audio clip to be played on this AudioSource
 * \param soundName
 */
void SetAudioClip(std::string_view audioClip);

/**
 * \brief Play the sound with specified properties
 * \param loop
 * \param volume
 */
void Play();

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
void SetVolume(float);
float GetVolume() const;
void Mute(bool = true);
/**
 * \brief Change speed of music, only supports MOD/S3M/XM/IT/MIDI file formats
 */
void SetSpeed(float);
float GetSpeed() const;
/**
 * \brief Check if the sound is currently playing
 * \return
 */
bool IsPlaying() const;

void LoopFor(int count);

private:
FMOD::Sound* fmodSound{};
FMOD::Channel* fmodChannel{};
static AudioModule* audioModule;

bool isChannelValid() const;
bool isSoundValid() const;

float volume, speed, loopCount;
std::bitset<3> properties;
std::string audioClip;

friend class AudioModule;
END_COMPONENT(AudioSource, Component);
}  // namespace Isetta
