/*
 * Copyright (c) 2018 Isetta
 */
#include "AudioClip.h"

#include "Audio/AudioModule.h"
#include "Core/Config/Config.h"
#include "Core/Memory/MemoryManager.h"
#include "FMOD/inc/fmod.hpp"
#include "SID/sid.h"

namespace Isetta {
AudioModule* AudioClip::audioModule;
std::unordered_map<StringId, AudioClip*> AudioClip::clips;

AudioClip::AudioClip(const std::string_view filePath,
                     const std::string_view name)
    : filePath{filePath}, name{name} {
  audioModule->LoadClip(this);

  // soundName defaults to filePath if empty
  if (name.empty()) this->name = filePath;
  clips.insert({SID(this->name.data()), this});
}

AudioClip* AudioClip::Load(const std::string_view filePath,
                           const std::string_view soundName) {
  StringId sid = SID(soundName.data());
  if (soundName.empty()) sid = SID(filePath.data());
  auto nameClipPair = clips.find(sid);
  if (nameClipPair == clips.end()) {
    return MemoryManager::NewOnFreeList<AudioClip>(filePath, soundName);
  }
  return nameClipPair->second;
}

AudioClip::~AudioClip() {
  AudioModule::CheckStatus(fmodSound->release());
  clips.erase(SID(name.c_str()));
}

AudioClip* AudioClip::Find(const std::string_view name) {
  auto nameClipPair = clips.find(SID(name.data()));
  if (nameClipPair == clips.end()) {
    LOG_ERROR(Debug::Channel::Sound,
              Util::StrFormat("Sound (%s) not found!", name.data()));
    return nullptr;
  }
  return nameClipPair->second;
}

void AudioClip::UnloadAll() {
  while (!clips.empty()) {
    auto nameClipPair = clips.begin();
    AudioClip* clip = nameClipPair->second;
    MemoryManager::DeleteOnFreeList<AudioClip>(clip);
  }
  clips.clear();
}
}  // namespace Isetta
