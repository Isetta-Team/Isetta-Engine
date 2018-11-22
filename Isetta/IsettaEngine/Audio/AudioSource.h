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
BEGIN_COMPONENT(AudioSource, Component, false)
public:
enum class Property {
  IS_3D,
  LOOP,
  IS_MUTE,
};
class AudioClip* clip{nullptr};

AudioSource();
explicit AudioSource(class AudioClip* clip);
AudioSource(const std::bitset<3>& properties, class AudioClip* clip);

void Update() override;

void SetProperty(Property prop, bool value);
bool GetProperty(Property prop) const;

void SetAudioClip(AudioClip* clip);
void Play();
// Pause the sound, preserving progress
void Pause() const;
void Continue() const;
void Stop() const;

void SetVolume(float);
float GetVolume() const;
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
