/*
 * Copyright (c) 2018 Isetta
 */
#include <chrono>
#include <iostream>
#include <string>
#include "Core/Audio/Audio.h"
#include "Core/Debug.h"
#include "Core/Math/Random.h"
#include "Core/Time.h"

namespace Isetta {

AudioSystem gAudioSystem;

void StartUp() {
  std::cout << "Initializing game" << std::endl;

  Debug::StartUp();
  gAudioSystem.StartUp();

  // Audio test code
  auto sing = AudioSource::LoadSound("singing.wav");
  sing->Play(true, 1.0f);
}

void Update() { gAudioSystem.Update(); }

void ShutDown() {
  std::cout << "Shutting down game" << std::endl;

  Debug::ShutDown();
  gAudioSystem.ShutDown();
}

}  // namespace Isetta

using namespace Isetta;

int main() {
  auto rnd = Isetta::Math::Random::GetRandomGenerator(1.f, 10.f);
  float number = rnd.GetValue();
  std::cout << number << std::endl;

  using clock = std::chrono::high_resolution_clock;
  using second = std::chrono::duration<float>;

  StartUp();

  Debug::PrintF(Debug::Memory, Debug::Info, "Hi %s, you are %d", "Jake", 10);
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
