/*
 * Copyright (c) 2018 Isetta
 */
#include <chrono>
#include <iostream>
#include <string>
#include "Core/Audio/Audio.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Random.h"
#include "Core/ModuleManager.h"
#include "Core/Time.h"

using namespace Isetta;

int main() {
  ModuleManager moduleManager;
  moduleManager.StartUp();

  // Random number test
  auto rnd = Isetta::Math::Random::GetRandomGenerator(1.f, 10.f);
  float number = rnd.GetValue();
  std::cout << number << std::endl;


  using clock = std::chrono::high_resolution_clock;
  using second = std::chrono::duration<float>;

  // Logger::PrintF(Debug::Memory, Debug::Info, "Hi %s, you are %d", "Jake", 10);
  // Logger::PrintF("Test\n");

  // Logging test
  // Logger::PrintF(Debug::Memory, Debug::Info, "Hi %s, you are %d", "Jake", 10);
  // Logger::PrintF("Test\n");

  Logger::PrintF(Debug::Memory, Debug::Info, "Hi %s, you are %d", "Jake", 10);
  Logger::PrintF("Test\n");
  // Game loop
  const float gameMaxDuration = 10.0f;

  using clock = std::chrono::high_resolution_clock;
  using second = std::chrono::duration<float>;

  Time::startTime = clock::now();
  auto lastFrameStartTime = clock::now();

  // play first audio clip
  auto audioSource = new AudioSource();
  audioSource->SetAudioClip("wave.mp3");

  audioSource->Play(true, 1.0f);
  std::cout << "Playing first" << std::endl;

  while (true) {
    Time::deltaTime = second(clock::now() - lastFrameStartTime).count();
    Time::time = second(clock::now() - Time::startTime).count();
    lastFrameStartTime = clock::now();

    moduleManager.Update();
    Time::frameCount++;

    // switch to playing the second audio clip
    if (Time::frameCount == 1000000) {
      audioSource->Stop();
      audioSource->SetAudioClip("singing.wav");
      audioSource->Play(false, 1.0f);
      std::cout << "Playing second" << std::endl;
    }

    if (Time::time > gameMaxDuration) {
      break;
    }
  }

  moduleManager.ShutDown();
  system("pause");
  return 0;
}
