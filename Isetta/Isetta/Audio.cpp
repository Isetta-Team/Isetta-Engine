#include "fmod.hpp"
#include <iostream>
#include <chrono>

using namespace std;

int FMOD_main() {
	using clock = std::chrono::high_resolution_clock;
	
	// initialization
	FMOD::System *fmodSystem = nullptr;
	System_Create(&fmodSystem);
	fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	unsigned int version;
	fmodSystem->getVersion(&version);
	cout << version << endl;
	
	// sound initialization
	FMOD::Sound* sound;
	const char* fileName = "singing.wav";
	fmodSystem->createSound(fileName, FMOD_DEFAULT, nullptr, &sound);

	FMOD::Channel* channel = nullptr;
	sound->setMode(FMOD_LOOP_NORMAL);
	fmodSystem->playSound(sound, nullptr, false, &channel);

	// loop
	long duration = 0;

	const auto gameStartTime = clock::now();
	auto frameStartTime = clock::now();

	const chrono::seconds maxRunningTime(10);

	while (true) {
		chrono::duration<double, milli> dt = clock::now() - frameStartTime;
		frameStartTime = clock::now();

		
		// update


		chrono::duration<double, milli> duration = clock::now() - gameStartTime;
		if (chrono::duration<double, milli>(maxRunningTime).count() < duration.count()) {
			break;
		}
	}

	cout << "Ending game" << endl;
	system("pause");
	return 0;

}
