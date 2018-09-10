/*
 * Copyright (c) 2018 Isetta
 */
#include <chrono>
#include <iostream>
#include <string>
#include "Core/Audio/Audio.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Graphics/LightNode.h"
#include "Core/Graphics/ModelNode.h"
#include "Core/Input/InputInterface.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector3.h"
#include "Core/ModuleManager.h"
#include "Core/Time.h"
#include "FileSystem.h"

using namespace Isetta;

/*! \mainpage Isetta Engine
Game engine development is a very wide field in the industry, but also a very
inaccessible one. Budding engineers are advised to just jump into development to
learn, and it's for this reason that the Isetta Engine project came to be.

Our team is building a game engine specialized for the twin-stick shooter genre.
Every step of the way we will be documenting our process through a dedicated
blog. Readers can hear from us personally and get an in-depth look at our
journey to create the engine. In addition, we will supplement our content
through regular interviews with industry professionals who have built their own
engines before.

Between our own hands-on process and sage advice from veteran engineers, we hope
to give newcomers a clearer representation of the engine-building process.
*/
int main() {
  FileSystem fds;
  system("pause");
  return 0;

  Config config;
  Logger::Log(Debug::Channel::General,
              config.vector3Var.GetV3Val().ToString().c_str());

  ModuleManager moduleManager;
  moduleManager.StartUp();

  // Random number test
  auto rnd = Isetta::Math::Random::GetRandomGenerator(1.f, 10.f);
  float number = rnd.GetValue();
  std::cout << number << std::endl;

  using clock = std::chrono::high_resolution_clock;
  using second = std::chrono::duration<float>;

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

  ModelNode car{"test/Low-Poly-Racing-Car.scene.xml",
                Isetta::Math::Vector3{0, -20, 0}, Isetta::Math::Vector3::zero,
                Isetta::Math::Vector3::one};

  LightNode light{"materials/light.material.xml",
                  Isetta::Math::Vector3{0, 200, 600},
                  Isetta::Math::Vector3{0, 0, 0}, Isetta::Math::Vector3::one};
  Input::RegisterKeyPressCallback(KeyCode::U,
                                  []() { std::cout << "U" << std::endl; });

  bool running{true};

  while (running) {
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

    if (Input::IsKeyPressed(KeyCode::ESCAPE)) {
      running = false;
    }
  }

  moduleManager.ShutDown();
  system("pause");
  return 0;
}
