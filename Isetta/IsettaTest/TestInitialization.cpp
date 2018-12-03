/*
 * Copyright (c) 2018 Isetta
 */

#include "Core/Memory/MemoryManager.h"
#include "CppUnitTest.h"
//#include "EngineLoop.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Isetta {
class TestInitialization {
 public:
  TestInitialization();
  // static void TestClassInitialize() { engineLoop.StartUp(); };
  // static void TestClassCleanup() { engineLoop.ShutDown(); };

  static void TestClassInitialize() {  };
  static void TestClassCleanup() {  };

 private:
  // static EngineLoop engineLoop;
  static MemoryManager memory;
};

TestInitialization::TestInitialization(){};

// EngineLoop TestInitialization::engineLoop;
MemoryManager TestInitialization::memory;

TEST_MODULE_INITIALIZE(ModuleInitialize) {
  TestInitialization::TestClassInitialize();
}

TEST_MODULE_CLEANUP(ModuleCleanup) { TestInitialization::TestClassCleanup(); }
}  // namespace Isetta