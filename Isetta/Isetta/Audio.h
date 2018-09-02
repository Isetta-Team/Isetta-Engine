#pragma once
#include "fmod.hpp"
// #include "fmod_errors.h"
#include <string>
#include <vector>

using namespace std;

namespace Audio {

	struct Vector3 {
		float x, y, z;
	};

	class AudioSource {
	public:
		explicit AudioSource(FMOD::Sound*);
		void Play(bool loop, float volume);
		// void Play3D(Vector3, bool loop, float volume);
		void Pause() const;
		void Continue() const;
		void Stop() const;
		void SetVolume(const float) const;
		void Erase();

		bool isErased;

	private:
		FMOD::Sound* fmodSound;
		FMOD::Channel* fmodChannel{};

		bool isChannelValid() const;
	};

	class AudioManager {

	public:
		static void Init(const string& filesRoot);
		static AudioManager& Instance();
		
		void Update() const;
		void ShutDown();

		AudioSource* LoadSound(const string& soundName) const;
		FMOD::Channel* PlayFMODSound(FMOD::Sound* sound, bool loop, float volume) const;
		void AddAudioSource(AudioSource* audioSource);

	private:
		static AudioManager* instance;
		FMOD::System* fmodSystem = nullptr;
		string soundFilesRoot;
		vector<AudioSource*> audioSources;
		
		unsigned int GetVersion() const;
	};

	void Init(const string &soundFilesRoot);
	void Update();
	void ShutDown();
	
	AudioSource* LoadSound(const string& soundName);
	string GetMemoryReport();
}
