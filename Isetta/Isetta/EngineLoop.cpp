/*
 * Copyright (c) 2018 Isetta
 */
#include "EngineLoop.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/ModuleManager.h"
#include "Core/Time/Clock.h"
#include "Graphics/LightNode.h"
#include "Graphics/ModelNode.h"
#include "Input/Input.h"
#include "Input/InputEnum.h"

namespace Isetta {

void RunYidiTest();

void EngineLoop::StartUp() {
  Logger();
  Config::Instance().Read("config.cfg");

  intervalTime = 1.0 / Config::Instance().loopConfig.maxFps.GetVal();
  maxSimulationCount = Config::Instance().loopConfig.maxSimCount.GetVal();

  moduleManager.StartUp();

  StartGameClock();
  isGameRunning = true;

  Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                  [&]() { isGameRunning = false; });

  // Game Init Part

  // Read scene from scene file
  ModelNode car{"test/Low-Poly-Racing-Car.scene.xml", Math::Vector3{0, -20, 0},
                Math::Vector3::zero, Math::Vector3::one};

  LightNode light{"materials/light.material.xml", Math::Vector3{0, 200, 600},
                  Math::Vector3::zero, Math::Vector3::one};

  // TODO(Chaojie) remove later into game logic

  // Register Input callback from player script
  U64 handleA, handleB, handleC;
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

  // RunYidiTest();
}

void EngineLoop::Update() {
  GetGameClock().UpdateTime();

  // TODO(All) Add networking update

  // Client part
  accumulateTime += GetGameClock().GetDeltaTime();

  for (int i = 0; i < maxSimulationCount && accumulateTime > intervalTime;
       i++) {
    moduleManager.SimulationUpdate(intervalTime);
    // LOG_INFO(Debug::Channel::General, std::to_string(accumulateTime));
    accumulateTime -= intervalTime;
  }

  // TODO(Chaojie) after scenegraph, save previous state for prediction
  moduleManager.RenderUpdate(0);
}

void EngineLoop::ShutDown() { moduleManager.ShutDown(); }
void EngineLoop::StartGameClock() const { GetGameClock(); }

void EngineLoop::Run() {
  StartUp();
  while (isGameRunning) {
    Update();
  }
  ShutDown();
}

Clock& EngineLoop::GetGameClock() {
  static Clock gameTime{};
  return gameTime;
}

void RunYidiTest() {
  const U32 count = 10;
  std::vector<ObjectHandle<U64>> arr;
  arr.reserve(count);
  for (U32 i = 0; i < count; i++) {
    auto ref = MemoryManager::NewDynamic<U64>();
    *ref = i;
    arr.push_back(ref);
  }

  for (U32 i = 0; i < count; i++) {
    MemoryManager::DeleteDynamic(arr[i]);
  }
}

}  // namespace Isetta
