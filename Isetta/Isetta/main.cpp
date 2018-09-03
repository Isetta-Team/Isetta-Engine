#include <iostream>
#include <chrono>
#include "Audio.h"
#include "Time.h"

using namespace std;

Audio::AudioSource* sing;

bool hasExecuted = false;
const float eventTime = 3.0;

void InitGame() {
	cout << "Initializing game" << endl;

	Audio::Init(R"(Resources\Sound\)");

	sing = Audio::LoadSound("singing.wav");
	sing->Play(false, 1.0f);
	cout << Audio::GetMemoryReport();
}

void Update() {
	// cout << Time::time << endl;
	Audio::Update();

	if (Time::time > eventTime && !hasExecuted) {
		cout << "Hit 3 seconds!" << endl;
		sing->SetVolume(0.3);
		// sing->Pause();
		// sing->Continue();
		// sing->Erase();
		hasExecuted = true;
	}
}

void ShutDown() {
	cout << "Shutting down game" << endl;

	Audio::ShutDown();
	cout << Audio::GetMemoryReport();
}

int main() {
	using clock = std::chrono::high_resolution_clock;
	using second = chrono::duration<float>;

	InitGame();

	const float gameMaxDuration = 10.0;

	Time::startTime = clock::now();
	auto lastFrameStartTime = clock::now();

	while (true) {
		Time::deltaTime = second(clock::now() - lastFrameStartTime).count();
		Time::time = second(clock::now() - Time::startTime).count();
		lastFrameStartTime = clock::now();

		Update();
		Time::frameCount++;

		if (Time::time > gameMaxDuration) {
			break;
		}
	}

	ShutDown();
	system("pause");
	return 0;
}
