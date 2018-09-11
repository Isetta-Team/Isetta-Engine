/*
 * Copyright (c) 2018 Isetta
 */
#include <chrono>
#include <string>
#include "Core/Audio/Audio.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Graphics/LightNode.h"
#include "Core/Graphics/ModelNode.h"
#include "Core/Input/InputInterface.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector3.h"
#include "Core/Memory/Memory.h"
#include "Core/ModuleManager.h"
#include "Core/Time.h"

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

  // PoolAllocator<U64> poolAllocator(1000);
  // U64* u1 = poolAllocator.Get();
  // *u1 = 0xffffffffffffffff;
  // U64* u2 = poolAllocator.Get();
  // *u2 = 0xffffffffffffffff;
  // U64* u3 = poolAllocator.Get();
  // *u3 = 0xffffffffffffffff;
  // U64* u4 = poolAllocator.Get();
  // *u4 = 0xffffffffffffffff;
  // U64* u5 = poolAllocator.Get();
  // *u5 = 0xffffffffffffffff;
  // poolAllocator.Free(u1);
  // poolAllocator.Free(u2);
  // u1 = poolAllocator.Get(); // will get the memory u2 was using
  // *u1 = 0xffffffffffffffff;
  // poolAllocator.Free(u3);
  // poolAllocator.Free(u4);
  // u1 = poolAllocator.Get(); // will get the memory u4 was using
  // *u1 = 0xffffffffffffffff;
  // poolAllocator.Free(u5);

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
