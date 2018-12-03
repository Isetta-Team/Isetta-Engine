/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector2.h"
#include "Scene/Component.h"

namespace FMOD {
class Channel;
}

namespace Isetta {
/**
 * @brief Component which plays AudioClips
 *
 */
DEFINE_COMPONENT(AudioSource, Component, false)
public:
enum class Property {
  IS_3D,
  LOOP,
  IS_MUTE,
};
/**
 * @brief Default AudioClip to play
 *
 */
class AudioClip* clip{nullptr};

/**
 * @brief Construct a new Audio Source object. Defaults to 3D audio
 *
 */
AudioSource() = default;
//
/**
 * @brief Construct a new Audio Source object. Deafult to 3D audio, not loop,
 * not mute, 1 volume, min 1, max 1000.
 *
 * @param clip default AudioClip
 *
 */
explicit AudioSource(class AudioClip* clip);
/**
 * @brief Construct a new Audio Source object
 *
 * @param properties format: 0b000
 *  0b(IS_MUTE)(LOOP)(IS_3D)
 * @param clip default AudioClip
 */
AudioSource(const std::bitset<3>& properties, class AudioClip* clip);

void Update() override;

/**
 * @brief Set a Property of AudioSource
 *
 * @param property to set
 * @param value to set property to
 */
void SetProperty(Property property, bool value);
/**
 * @brief Get a Property of AudioSource
 *
 * @param property to get
 */
bool GetProperty(Property property) const;

/**
 * @brief Play the audio
 *
 */
void Play();
/**
 * @brief Pause the audio
 *
 */
void Pause() const;
/**
 * @brief Continue the audio
 *
 */
void Continue() const;
/**
 * @brief Stop the audio
 *
 */
void Stop() const;

/**
 * @brief Set the Volume
 *
 */
void SetVolume(float);
/**
 * @brief Get the Volume
 *
 */
float GetVolume() const;

/**
 * @brief Set the Min Max Distance in 3D sound can be heard
 *
 * @param minMax distance which sound can be heard
 */
void SetMinMaxDistance(Math::Vector2 minMax);
/**
 * @brief Get the Min Max Distance
 *
 * @return Math::Vector2
 */
Math::Vector2 GetMinMaxDistance() const;

/**
 * \brief Change speed of music, only supports MOD/S3M/XM/IT/MIDI file formats
 */
// void SetSpeed(float);
// float GetSpeed() const;

/**
 * @brief Check if the audio is playing
 */
bool IsPlaying() const;
/**
 * @brief Check if the audio is paused
 *
 */
bool IsPaused() const;
/**
 * @brief Check if audio is started
 *
 */
bool IsStarted() const;

/**
 * @brief Number of loops to do before stopping.
 *  -1: infinite
 *  0: one shot
 *
 * @param count loop times
 */
void LoopFor(int count);

private:
FMOD::Channel* fmodChannel{};

bool IsChannelValid() const;
bool IsSoundValid() const;

float volume = 1.f, speed = 1.f;
int loopCount = -1;
Math::Vector2 minMaxDistance = Math::Vector2{1.f, 1000.f};
std::bitset<3> properties = 0b001;

static class AudioModule* audioModule;
friend class AudioModule;
DEFINE_COMPONENT_END(AudioSource, Component);
}  // namespace Isetta
