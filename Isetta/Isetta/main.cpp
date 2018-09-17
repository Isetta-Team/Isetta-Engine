/*
 * Copyright (c) 2018 Isetta
 */
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <string>
#include "Audio/AudioSource.h"
#include "Core/Debug/Logger.h"
#include "Core/FileSystem.h"
#include "Core/Math/Random.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/Memory/StackAllocator.h"
#include "Core/Memory/TemplatePoolAllocator.h"
#include "EngineLoop.h"
#include "Input/Input.h"
#include "yojimbo/shared.h"

using namespace Isetta;

void RunBenchmarks();
void RunYidiTest();

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
  EngineLoop loop;
  loop.Run();

  // auto h = FileSystem::Instance().Read(
  //    "Resources/test/async.in",
  //    std::function<void(const char *)>(
  //        [](const char *buf) { printf("%s\n", buf); }));
  // auto h = FileSystem::Instance().Read("Resources/test/async.in");

  // char* buf = "abcdefghijklmnopqrstuvwxyz\n";
  // FileSystem::Instance().Write(
  //    "Resources/test/async.out", buf,
  //    std::function<void(const char*)>(
  //        [](const char* buf) { printf("> write done\n"); }),
  //    false);

  // RunBenchmarks();
  RunYidiTest();

  return 0;
}

void RunBenchmarks() {
  using clock = std::chrono::high_resolution_clock;
  typedef std::chrono::duration<float> second;

  std::unordered_map<std::string, std::function<void()>> benchmarks;

  benchmarks.insert({"1. new and delete", []() {
                       const int count = 10000;
                       AudioSource *audioSources[count];
                       for (auto &audioSource : audioSources) {
                         audioSource = new AudioSource();
                       }
                       for (auto &audioSource : audioSources) {
                         delete audioSource;
                       }
                     }});

  benchmarks.insert({"2. malloc and free", []() {
                       const int count = 10000;
                       AudioSource *audioSources[count];
                       for (auto &audioSource : audioSources) {
                         audioSource = new (std::malloc(sizeof(AudioSource)))
                             AudioSource();
                       }
                       for (auto &audioSource : audioSources) {
                         std::free(audioSource);
                       }
                     }});

  benchmarks.insert({"3. Stack Allocator", []() {
                       const int count = 10000;
                       AudioSource *audioSources[count];
                       StackAllocator stackAllocator(sizeof(AudioSource) *
                                                     count);
                       for (auto &audioSource : audioSources) {
                         audioSource = stackAllocator.New<AudioSource>();
                       }
                     }});

  benchmarks.insert({"4. Pool Allocator", []() {
                       const int count = 10000;
                       AudioSource *audioSources[count];
                       TemplatePoolAllocator<AudioSource> poolAllocator(count);
                       for (auto &audioSource : audioSources) {
                         audioSource = poolAllocator.Get();
                       }

                       for (auto &audioSource : audioSources) {
                         poolAllocator.Free(audioSource);
                       }
                     }});

  // Benchmarking
  const int testIterations = 10;
  for (auto &test : benchmarks) {
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

void RunYidiTest() {
  auto &audio = MemoryManager::NewDynamic<AudioSource>();

  const U32 count = 10;
  std::vector<ObjectHandle<U64> *> arr;
  arr.reserve(count);
  for (U32 i = 0; i < count; i++) {
    auto &ref = MemoryManager::NewDynamic<U64>();
    *ref = i;
    arr.push_back(&ref);
  }

  for (U32 i = 0; i < count; i++) {
    MemoryManager::FreeDynamic(*arr[i]);
  }

  MemoryManager::FreeDynamic(audio);

  // auto &hi = MemoryManager::NewDynamic<U64>();
  // MemoryManager::FreeDynamic(hi);
}
