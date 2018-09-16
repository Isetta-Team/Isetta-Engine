/*
 * Copyright (c) 2018 Isetta
 */
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <string>
#include "Audio/AudioSource.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/FileSystem.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector3.h"
#include "Core/Memory/DoubleBufferedAllocator.h"
#include "Core/Memory/PoolAllocator.h"
#include "Core/Memory/StackAllocator.h"
#include "Core/Memory/TemplatePoolAllocator.h"
#include "Core/ModuleManager.h"
#include "Core/Time/Clock.h"
#include "Graphics/LightNode.h"
#include "Graphics/ModelNode.h"
#include "Input/Input.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/Memory/MemoryManager.h"

using namespace Isetta;

void RunYidiTestCases();
void RunBenchmarks();

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
  // TODO(Chaojie): maybe move to loop class later
  Clock gameTime{};

  Config config;
  config.Read("config.cfg");
  LOG_INFO(Debug::Channel::General,
           config.vector3Var.GetVal().ToString().c_str());

  ModuleManager moduleManager;
  moduleManager.StartUp();

  RunYidiTestCases();

  // Random number test
  // auto rnd = Math::Random::GetRandomGenerator(1.f, 10.f);
  // float number = rnd.GetValue();
  // Logger::Log(Debug::Channel::General,
  // "Random number: " + std::to_string(number));

  using clock = std::chrono::high_resolution_clock;
  typedef std::chrono::duration<float> second;

  // RunBenchmarks();

  U64 handleA, handleB, handleC, handleD;
  handleA = Input::RegisterKeyPressCallback(KeyCode::A, [&handleA]() {
    LOG_INFO(Debug::Channel::General, "A pressed");
    Input::UnregisterKeyPressCallback(KeyCode::A, handleA);
  });
  handleB = Input::RegisterKeyReleaseCallback(KeyCode::A, [&handleB]() {
    LOG_INFO(Debug::Channel::General, "A released");
    Input::UnregisterKeyReleaseCallback(KeyCode::A, handleB);
  });
  handleC = Input::RegisterMousePressCallback(
      MouseButtonCode::MOUSE_LEFT, [&handleC]() {
        LOG_INFO(Debug::Channel::General,
                 {"Left pressed at: " + Input::GetMousePosition().ToString()});
        Input::UnregisterMousePressCallback(MouseButtonCode::MOUSE_LEFT,
                                            handleC);
      });

  // Game loop
  auto lastFrameStartTime = clock::now();

  ModelNode car{"test/Low-Poly-Racing-Car.scene.xml", Math::Vector3{0, -20, 0},
                Math::Vector3::zero, Math::Vector3::one};

  LightNode light{"materials/light.material.xml", Math::Vector3{0, 200, 600},
                  Math::Vector3::zero, Math::Vector3::one};

  bool running{true};

  while (running) {
    gameTime.UpdateTime();

    moduleManager.Update(gameTime.GetDeltaTime());

    if (Input::IsKeyPressed(KeyCode::ESCAPE)) {
      running = false;
    }
  }

  moduleManager.ShutDown();
  return 0;
}

void RunYidiTestCases() {

}

void RunBenchmarks() {
  using clock = std::chrono::high_resolution_clock;
  typedef std::chrono::duration<float> second;

  std::unordered_map<std::string, std::function<void()>> benchmarks;

  benchmarks.insert({"1. new and delete", []() {
                       const int count = 10000;
                       AudioSource* audioSources[count];
                       for (auto& audioSource : audioSources) {
                         audioSource = new AudioSource();
                       }
                       for (auto& audioSource : audioSources) {
                         delete audioSource;
                       }
                     }});

  benchmarks.insert({"2. malloc and free", []() {
                       const int count = 10000;
                       AudioSource* audioSources[count];
                       for (auto& audioSource : audioSources) {
                         audioSource = new (std::malloc(sizeof(AudioSource)))
                             AudioSource();
                       }
                       for (auto& audioSource : audioSources) {
                         std::free(audioSource);
                       }
                     }});

  benchmarks.insert({"3. Stack Allocator", []() {
                       const int count = 10000;
                       AudioSource* audioSources[count];
                       StackAllocator stackAllocator(sizeof(AudioSource) *
                                                     count);
                       for (auto& audioSource : audioSources) {
                         audioSource = stackAllocator.New<AudioSource>();
                       }
                       stackAllocator.Erase();
                     }});

  benchmarks.insert({"4. Template Pool Allocator", []() {
                       const int count = 10000;
                       AudioSource* audioSources[count];
                       TemplatePoolAllocator<AudioSource> poolAllocator(count);
                       for (auto& audioSource : audioSources) {
                         audioSource = poolAllocator.Get();
                       }

                       for (auto& audioSource : audioSources) {
                         poolAllocator.Free(audioSource);
                       }
                       poolAllocator.Erase();
                     }});

  // Benchmarking
  const int testIterations = 10;
  for (auto& test : benchmarks) {
    float time = 0;
    for (int a = 0; a < testIterations; a++) {
      const auto benchmarkStart = clock::now();
      test.second();
      const auto benchmarkEnd = clock::now();
      time += second(benchmarkEnd - benchmarkStart).count();
    }
    std::string duration = std::to_string((time / testIterations)) + "s";
    LOG_INFO(Debug::Channel::Memory, {test.first + ": " + duration});
  }
}
