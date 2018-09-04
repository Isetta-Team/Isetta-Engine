#include <iostream>
#include <string>
#include <chrono>
#include "Core/Time.h"
#include "Audio.h"
#include "Math/Random.h"
#include "Time.h"

#include "Core/Debug.h"

using namespace Isetta;

void StartUp() {
    std::cout << "Initializing game" << std::endl;
    Debug::StartUp();
}

void Update() {
    // cout << Time::time << endl;

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
    std::cout << "Shutting down game" << std::endl;
    Debug::ShutDown();
}

int main() {
  auto rnd = Isetta::Math::Random::GetRandomGenerator(1.f, 10.f);
  float number = rnd.GetValue();
  std::cout << number << std::endl;

  using clock = std::chrono::high_resolution_clock;
  using second = std::chrono::duration<float>;

    StartUp();

    Debug::PrintF(Debug::Memory, Debug::Info, "Hi %s, you are %d", "Yidi", 10);
    Debug::PrintF("Test\n");

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
