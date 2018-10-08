/*
 * Copyright (c) 2018 Isetta
 */

#include <iostream>

#include "CppUnitTest.h"
#include "EngineLoop.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Isetta {
class TestInitialization {
 public:
  TestInitialization();
  static void TestClassInitialize() { engineLoop.StartUp(); };
  static void TestClassCleanup() { engineLoop.ShutDown(); };

 private:
  static EngineLoop engineLoop;
};

TestInitialization::TestInitialization(){};

EngineLoop TestInitialization::engineLoop;

TEST_MODULE_INITIALIZE(ModuleInitialize) {
  TestInitialization::TestClassInitialize();
}

TEST_MODULE_CLEANUP(ModuleCleanup) {
  TestInitialization::TestClassCleanup();
}
}  // namespace Isetta