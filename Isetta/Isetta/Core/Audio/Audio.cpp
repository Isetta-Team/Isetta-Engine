#include "Audio.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include "combaseapi.h"

namespace Isetta {

AudioSource* AudioSource::LoadSound(const std::string& soundName) {
	return gAudioSystem.LoadSound(soundName);
}

AudioSource::AudioSource(FMOD::Sound* sound) {
	fmodSound = sound;
	isErased = false;
	gAudioSystem.AddAudioSource(this);
}

void AudioSource::Play(const bool loop, const float volume) {
	fmodChannel = gAudioSystem.PlayFMODSound(fmodSound, loop, volume);
}

void AudioSource::Pause() const {
	if (isChannelValid()) {
		fmodChannel->setPaused(true);
	}
}

void AudioSource::Continue() const {
	if (isChannelValid()) {
		fmodChannel->setPaused(false);
	}
}

void AudioSource::Stop() const {
	if (isChannelValid()) {
		fmodChannel->stop();
	}
}

void AudioSource::SetVolume(const float volume) const {
	if (isChannelValid()) {
		fmodChannel->setVolume(volume);
	}
}

void AudioSource::Erase() {
	Stop();
	isErased = true;
	fmodSound->release();
}

bool AudioSource::isChannelValid() const {
	bool isPlaying = false;
	fmodChannel->isPlaying(&isPlaying);
	return fmodChannel != nullptr && isPlaying;
}

void AudioSystem::StartUp() {
	fmodSystem = nullptr;
	FMOD::System_Create(&fmodSystem);
	fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);
	// TODO: Set this in engine config
	soundFilesRoot = R"(Resources\Sound\)";
}

void AudioSystem::Update() const { fmodSystem->update(); }

void AudioSystem::ShutDown() {
	for (auto it : audioSources) {
		it->Erase();
		it->isErased = true;
		delete (it);
	}

	audioSources.clear();
	fmodSystem->release();
	CoUninitialize();
}

AudioSource* AudioSystem::LoadSound(const std::string& soundName) const {
	FMOD::Sound* sound = nullptr;
	std::string path = soundFilesRoot + soundName;
	std::cout << "Loading: " << path << std::endl;
	fmodSystem->createSound(path.c_str(), FMOD_LOWMEM, nullptr, &sound);

	return new AudioSource(sound);
}

FMOD::Channel* AudioSystem::PlayFMODSound(FMOD::Sound* sound, bool loop,
                                          float volume) const {
	FMOD::Channel* channel = nullptr;
	sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	fmodSystem->playSound(sound, nullptr, false, &channel);
	channel->setVolume(volume);
	return channel;
}

void AudioSystem::AddAudioSource(AudioSource* audioSource) {
	audioSources.push_back(audioSource);
}

inline float MegaBytesFromBytes(const int byte) {
	return byte / 1024.f / 1024.f;
}

std::string AudioSystem::GetMemoryReport() {
	int currentAllocated;
	int maxAllocated;
	FMOD::Memory_GetStats(&currentAllocated, &maxAllocated);

	std::stringstream memReport;
	memReport << std::fixed << std::setprecision(2)
		<< "------------- Audio Engine Memory Usage ------------------"
		<< std::endl
		<< "Current usage: " << MegaBytesFromBytes(currentAllocated)
		<< "mb, Max usage: " << MegaBytesFromBytes(maxAllocated) << "mb.\n"
		<< std::endl;

	return memReport.str();
}
} // namespace Isetta
