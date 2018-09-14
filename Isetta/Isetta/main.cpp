/*
 * Copyright (c) 2018 Isetta
 */
#include <chrono>
#include <string>
#include "Audio/AudioSource.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/FileSystem.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector3.h"
#include "Core/Memory/Memory.h"
#include "Core/ModuleManager.h"
#include "Core/Time.h"
#include "Graphics/LightNode.h"
#include "Graphics/ModelNode.h"
#include "Input/Input.h"

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

  FileSystem::OverlapIOInfo* readO = new FileSystem::OverlapIOInfo{};
  readO->callback = std::function<void(const char*)>(
      [](const char* buf) { printf("%s\n", buf); });
  fds.Read("Resources/test/async.in", readO);

  for (int i = 0; i < 5; i++) {
    FileSystem::OverlapIOInfo* writeO = new FileSystem::OverlapIOInfo{};
    writeO->buf = "abcdefghijklmnopqrstuvwxyz\n";
    fds.Write("Resources/test/async.out", writeO);
  }
  system("pause");
  return 0;

  Config config;
  config.Read("config.cfg");
  LOG_INFO(Debug::Channel::General,
           config.vector3Var.GetVal().ToString().c_str());

  ModuleManager moduleManager;
  moduleManager.StartUp();

  // Random number test
  // auto rnd = Math::Random::GetRandomGenerator(1.f, 10.f);
  // float number = rnd.GetValue();
  // Logger::Log(Debug::Channel::General,
  // "Random number: " + std::to_string(number));

  using clock = std::chrono::high_resolution_clock;
  typedef std::chrono::duration<float> second;

  auto audio = new AudioSource();
  audio->SetAudioClip("singing.wav");
  audio->Play(false, 1.0f);

  // Benchmarking
  const int testIterations = 10;
  for (int a = 0; a < testIterations; a++) {
    const auto benchmarkStart = clock::now();
    // benchmark code here...
    const auto benchmarkEnd = clock::now();
    LOG_INFO(
        Debug::Channel::Memory,
        {"Bench mark result: ",
         std::to_string(second(benchmarkEnd - benchmarkStart).count()), "s"});
  }

  // Game loop
  Time::startTime = clock::now();
  auto lastFrameStartTime = clock::now();

  ModelNode car{"test/Low-Poly-Racing-Car.scene.xml", Math::Vector3{0, -20, 0},
                Math::Vector3::zero, Math::Vector3::one};

  LightNode light{"materials/light.material.xml", Math::Vector3{0, 200, 600},
                  Math::Vector3::zero, Math::Vector3::one};

  bool running{true};

  while (running) {
    Time::deltaTime = second(clock::now() - lastFrameStartTime).count();
    Time::time = second(clock::now() - Time::startTime).count();
    lastFrameStartTime = clock::now();

    moduleManager.Update();
    Time::frameCount++;

    if (Input::IsKeyPressed(KeyCode::ESCAPE)) {
      running = false;
    }
  }

  moduleManager.ShutDown();
  return 0;
}
