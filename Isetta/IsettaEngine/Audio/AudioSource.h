/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Scene/Component.h"

namespace FMOD {
class Channel;
}

namespace Isetta {
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
class AudioClip* clip;

AudioSource();
explicit AudioSource(std::string_view clipName);
explicit AudioSource(class AudioClip* clip);
AudioSource(const std::bitset<3>& properties, std::string_view clipName = "");
AudioSource(const std::bitset<3>& properties, class AudioClip* clip);

void Update() override;

void SetProperty(const Property prop, bool value);
bool GetProperty(const Property prop) const;

/**
 * \brief Set the audio clip to be played on this AudioSource
 * \param soundName
 */
void SetAudioClip(const std::string_view audioClip);

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
// void SetSpeed(float);
// float GetSpeed() const;
/**
 * \brief Check if the sound is currently playing
 * \return
 */
bool IsPlaying() const;
bool IsPaused() const;
bool IsStarted() const;

void LoopFor(int count);

private:
FMOD::Channel* fmodChannel{};

bool IsChannelValid() const;
bool IsSoundValid() const;
bool IsValidHandle() const;

float volume, speed;
int loopCount;
std::bitset<3> properties;

static class AudioModule* audioModule;
friend class AudioModule;
END_COMPONENT(AudioSource, Component);
}  // namespace Isetta
