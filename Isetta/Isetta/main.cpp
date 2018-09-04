#include "Audio.h"
#include "Math/Random.h"
#include "Time.h"
#include <chrono>
#include <iostream>

Isetta::AudioSource *sing;
bool hasExecuted = false;
const float eventTime = 3.0;

void InitGame() {
  std::cout << "Initializing game" << std::endl;

  Isetta::AudioSystem::Init(R"(Resources\Sound\)");

  sing = Isetta::AudioSystem::LoadSound("singing.wav");
  sing->Play(false, 1.0f);
  std::cout << Isetta::AudioSystem::GetMemoryReport();
}

void Update() {
  // cout << Time::time << endl;
  Isetta::AudioSystem::Update();

  if (Time::time > eventTime && !hasExecuted) {
    std::cout << "Hit 3 seconds!" << std::endl;
    sing->SetVolume(0.3f);
    // sing->Pause();
    // sing->Continue();
    // sing->Erase();
    hasExecuted = true;
  }
}

void ShutDown() {
  std::cout << "Shutting down game" << std::endl;

  Isetta::AudioSystem::ShutDown();
  std::cout << Isetta::AudioSystem::GetMemoryReport();
}

int main() {
  auto rnd = Isetta::Math::Random::GetRandomGenerator(1.f, 10.f);
  float number = rnd.GetValue();
  std::cout << number << std::endl;

  using clock = std::chrono::high_resolution_clock;
  using second = std::chrono::duration<float>;

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