#include "Audio.h"
#include "combaseapi.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

namespace Audio {
	AudioManager* AudioManager::instance;

	AudioSource::AudioSource(FMOD::Sound* sound) {
		fmodSound = sound;
		isErased = false;
		AudioManager::Instance().AddAudioSource(this);
	}

	void AudioSource::Play(const bool loop, const float volume) {
		fmodChannel = AudioManager::Instance().PlayFMODSound(fmodSound, loop, volume);
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

	void AudioManager::Init(const string& filesRoot) {
		instance = new AudioManager();
		instance->fmodSystem = nullptr;
		FMOD::System_Create(&instance->fmodSystem);
		instance->fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);
		instance->soundFilesRoot = filesRoot;
	}

	AudioManager& AudioManager::Instance() {
		return *instance;
	}

	void AudioManager::Update() const {
		fmodSystem->update();
	}

	void AudioManager::ShutDown() {
		for (auto it : audioSources) {
			it->Erase();
			it->isErased = true;
			delete(it);
		}

		audioSources.clear();
		fmodSystem->release();
		CoUninitialize();
	}

	AudioSource* AudioManager::LoadSound(const string& soundName) const {
		FMOD::Sound* sound = nullptr;
		string path = soundFilesRoot + soundName;
		cout << "Loading: " << path << endl;
		fmodSystem->createSound(path.c_str(), FMOD_LOWMEM, nullptr, &sound);

		return new AudioSource(sound);
	}

	FMOD::Channel* AudioManager::PlayFMODSound(FMOD::Sound* sound, bool loop, float volume) const {
		FMOD::Channel* channel = nullptr;
		sound->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
		fmodSystem->playSound(sound, nullptr, false, &channel);
		channel->setVolume(volume);
		return channel;
	}

	void AudioManager::AddAudioSource(AudioSource* audioSource) {
		audioSources.push_back(audioSource);
	}

	unsigned AudioManager::GetVersion() const {
		unsigned int version;
		fmodSystem->getVersion(&version);
		return version;
	}

	void Init(const string &soundFilesRoot) {
		// CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		// const int bytes = 4 * 1024 * 1024;
		// FMOD::Memory_Initialize(malloc(bytes), bytes, nullptr, nullptr, nullptr);
		AudioManager::Init(soundFilesRoot);
	}

	void Update() {
		AudioManager::Instance().Update();
	}

	void ShutDown() {
		AudioManager::Instance().ShutDown();
		delete(&AudioManager::Instance());
	}

	AudioSource* LoadSound(const string& soundName) {
		return AudioManager::Instance().LoadSound(soundName);
	}

	inline float MegaBytesFromBytes(const int byte) {
		return byte / 1024.0 / 1024.0;
	}

	string GetMemoryReport() {
		int currentAllocated;
		int maxAllocated;
		FMOD::Memory_GetStats(&currentAllocated, &maxAllocated);
		
		stringstream memReport;
		memReport << fixed << setprecision(2)
		<< "------------- Audio Engine Memory Usage ------------------" << endl
		<< "Current usage: " << MegaBytesFromBytes(currentAllocated) 
		<< "mb, Max usage: " << MegaBytesFromBytes(maxAllocated) << "mb.\n" << endl;

		return memReport.str();
	}
}
