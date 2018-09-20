/*
 * Copyright (c) 2018 Isetta
 */
#include "EngineLoop.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/ModuleManager.h"
#include "Core/Time/Clock.h"
#include "Graphics/AnimationNode.h"
#include "Graphics/LightNode.h"
#include "Graphics/ModelNode.h"
#include "Input/Input.h"
#include "Input/InputEnum.h"
#include "Networking/Messages.h"
#include "Networking/NetworkManager.h"

namespace Isetta {

void RunYidiTest();

void EngineLoop::StartUp() {
  Logger::NewSession();
  Config::Instance().Read("config.cfg");

  intervalTime = 1.0 / Config::Instance().loopConfig.maxFps.GetVal();
  maxSimulationCount = Config::Instance().loopConfig.maxSimCount.GetVal();

  moduleManager.StartUp();

  StartGameClock();
  isGameRunning = true;

  Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                  [&]() { isGameRunning = false; });

  // Game Init Part

  // Networking
  NetworkManager::CreateServer("127.0.0.1");
  NetworkManager::ConnectToServer("127.0.0.1", [](bool b) {
    LOG(Debug::Channel::Networking, "Client connection state: %d", b);
  });

  // TODO(All) Read scene from scene file
  // ModelNode car{"test/Low-Poly-Racing-Car.scene.xml", Math::Vector3{0, -20,
  // 0},
  //              Math::Vector3::zero, Math::Vector3::one};
  static ModelNode car{"push/Pushing.scene.xml", Math::Vector3{-200, -100, 0},
                       Math::Vector3{0, 90, 0}, Math::Vector3::one};
  static AnimationNode animation{&car};
  animation.AddAnimation("push/Pushing.anim", 0, "", false);
  animation.Play();
  LightNode light{"materials/light.material.xml", Math::Vector3{0, 200, 600},
                  Math::Vector3::zero, Math::Vector3::one};

  // TODO(Chaojie) remove later into game logic

  // Register Input callback from player script
  U64 handleA, handleB, handleC;
  handleA = Input::RegisterKeyPressCallback(KeyCode::NUM1, [&handleA]() {
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

  Input::RegisterKeyPressCallback(KeyCode::P, []() {
    if (NetworkManager::ClientIsConnected()) {
      NetworkManager::SendHandleMessageFromClient(0);
    }
  });
  Input::RegisterKeyPressCallback(KeyCode::O, []() {
    if (NetworkManager::ClientIsConnected()) {
      NetworkManager::SendHandleMessageFromClient(1);
    }
  });
  Input::RegisterMousePressCallback(MouseButtonCode::MOUSE_LEFT, []() {
    if (NetworkManager::ClientIsConnected()) {
      NetworkManager::SendHandleMessageFromClient(2);
    }
  });

  // RunYidiTest();
}

void EngineLoop::Update() {
  GetGameClock().UpdateTime();

  // TODO(All) Add networking update

  static bool sIsPressed = false;
  static bool cIsPressed = false;

  // Networking update
  if (Input::IsKeyPressed(KeyCode::S)) {
    if (!sIsPressed) {
      sIsPressed = true;
      if (NetworkManager::ServerIsRunning()) {
        NetworkManager::SendStringMessageFromServer(0, "Hi!");
      }
    }
  } else {
    sIsPressed = false;
  }
  if (Input::IsKeyPressed(KeyCode::C)) {
    if (!cIsPressed) {
      cIsPressed = true;
      if (NetworkManager::ClientIsConnected()) {
        NetworkManager::SendStringMessageFromClient("Hi!");
      }
    }
  } else {
    cIsPressed = false;
  }

  // end Networking update

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
