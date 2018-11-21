/*
 * Copyright (c) 2018 Isetta
 */
#include "AudioClip.h"

#include "Audio/AudioModule.h"
#include "Core/Config/Config.h"
#include "Core/DataStructures/Array.h"
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
  if (name.empty()) this->name = filePath;
  clips.insert({SID(this->name.c_str()), this});
}
AudioClip::~AudioClip() {
  audioModule->CheckStatus(fmodSound->release());
  clips.erase(SID(name.c_str()));
  // MemoryManager::FreeOnFreeList(this);
}

AudioClip* AudioClip::GetClip(const std::string_view name) {
  auto clip = clips.find(SID(name.data()));
  return clip == clips.end() ? nullptr : clip->second;
}

void AudioClip::LoadConfigClips() {
  std::string clipNames = CONFIG_VAL(audioConfig.audioClips);
  Util::StrRemoveSpaces(&clipNames);
  Array<std::string> configClips;
  if (!clipNames.empty()) configClips = Util::StrSplit(clipNames, ',');

  for (const auto& clipName : configClips) {
    // AudioClip(clipName.c_str());
    MemoryManager::NewOnFreeList<AudioClip>(clipName.c_str());
  }
}
void AudioClip::UnloadAll() {
  while (clips.size() > 0) {
    auto it = clips.begin();
    AudioClip* clip = it->second;
    clips.erase(it);
    MemoryManager::DeleteOnFreeList<AudioClip>(clip);
  }
  clips.clear();
}
}  // namespace Isetta
