/*
 * Copyright (c) 2018 Isetta
 */
#include "EngineLoop.h"

#include "Audio/AudioModule.h"
#include "Collisions/CollisionsModule.h"
#include "Core/Memory/MemoryManager.h"
#include "Graphics/GUIModule.h"
#include "Graphics/RenderModule.h"
#include "Graphics/Window.h"
#include "Input/InputModule.h"
#include "Networking/NetworkingModule.h"

#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Filesystem.h"
#include "Core/Time/Clock.h"
#include "Graphics/GUI.h"
#include "Input/Input.h"
#include "Input/InputEnum.h"
#include "Networking/ExampleMessages.h"
#include "Networking/NetworkManager.h"
#include "Scene/Level.h"

#include "Scene/Entity.h"
#include "Scene/LevelManager.h"
#include "imgui/imgui.h"

#include "Core/Debug/DebugDraw.h"

namespace Isetta {

void InputDemo();
void NetworkingDemo();
void NetworkingDemoEnd();

EngineLoop::EngineLoop() {
  memoryManager = new MemoryManager{};
  audioModule = new AudioModule{};
  windowModule = new WindowModule{};
  renderModule = new RenderModule{};
  inputModule = new InputModule{};
  guiModule = new GUIModule{};
  networkingModule = new NetworkingModule{};
  collisionsModule = new CollisionsModule{};
}
EngineLoop::~EngineLoop() {
  delete windowModule;
  delete audioModule;
  delete renderModule;
  delete inputModule;
  delete guiModule;
  delete memoryManager;
  delete networkingModule;
  delete collisionsModule;
}

void EngineLoop::StartUp() {
  Logger::NewSession();
  Config::Instance().Read("config.cfg");
  if (Filesystem::Instance().FileExists("user.cfg")) {
    Config::Instance().Read("user.cfg");
  }

  intervalTime = 1.0 / Config::Instance().loopConfig.maxFps.GetVal();
  maxSimulationCount = Config::Instance().loopConfig.maxSimCount.GetVal();

  memoryManager->StartUp();
  windowModule->StartUp();
  renderModule->StartUp(windowModule->winHandle);
  inputModule->StartUp(windowModule->winHandle);
  guiModule->StartUp(windowModule->winHandle);
  collisionsModule->StartUp();
  audioModule->StartUp();
  networkingModule->StartUp();

  LevelManager::Instance().LoadStartupLevel();

  DebugDraw::StartUp();

  StartGameClock();
  isGameRunning = true;

  Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                  [&]() { isGameRunning = false; });

  // NetworkingDemo();
  // InputDemo();
  // RunYidiTest();
  // GraphicsDemo();
}

void EngineLoop::Update() {
  GetGameClock().UpdateTime();

  // TODO(All) Add networking update

  // end Networking update
  // LOG_INFO(Debug::Channel::General,
  // "//////////////UpdateStart//////////////");

  // Client part
  accumulateTime += GetGameClock().GetDeltaTime();

  for (int i = 0; i < maxSimulationCount && accumulateTime > intervalTime;
       i++) {
    FixedUpdate(intervalTime);
    // LOG_INFO(Debug::Channel::General,
    // std::to_string(accumulateTime));
    accumulateTime -= intervalTime;
  }

  VariableUpdate(GetGameClock().GetDeltaTime());
  // LOG_INFO(Debug::Channel::General,
  // "//////////////UpdateEnd//////////////");
}

void EngineLoop::FixedUpdate(float deltaTime) {
  networkingModule->Update(deltaTime);
  collisionsModule->Update(deltaTime);
}
void EngineLoop::VariableUpdate(float deltaTime) {
  inputModule->Update(deltaTime);
  LevelManager::Instance().currentLevel->Update();
  LevelManager::Instance().currentLevel->LateUpdate();
  audioModule->Update(deltaTime);
  renderModule->Update(deltaTime);
  DebugDraw::Update();
  guiModule->Update(deltaTime);
  windowModule->Update(deltaTime);
  memoryManager->Update();
}

void EngineLoop::ShutDown() {
  LevelManager::Instance().currentLevel->UnloadLevel();
  NetworkingDemoEnd();
  networkingModule->ShutDown();
  audioModule->ShutDown();
  collisionsModule->ShutDown();
  guiModule->ShutDown();
  inputModule->ShutDown();
  DebugDraw::ShutDown();
  renderModule->ShutDown();
  windowModule->ShutDown();
  memoryManager->ShutDown();
}

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

void InputDemo() {
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
}
void NetworkingDemo() {
  // Networking
  NetworkingExample::RegisterExampleMessageFunctions();

  if (Config::Instance().networkConfig.runServer.GetVal()) {
    NetworkManager::CreateServer(
        Config::Instance().networkConfig.defaultServerIP.GetVal().c_str());
  }
  if (Config::Instance().networkConfig.connectToServer.GetVal()) {
    NetworkManager::ConnectToServer(
        Config::Instance().networkConfig.defaultServerIP.GetVal().c_str(),
        [](bool b) {
          LOG(Debug::Channel::Networking, "Client connection state: %d", b);
        });
  }

  Input::RegisterKeyPressCallback(KeyCode::Y, []() {
    if (NetworkManager::ClientIsConnected()) {
      SpawnExample* m = reinterpret_cast<SpawnExample*>(
          NetworkManager::GenerateMessageFromClient("SPWN"));
      m->a = 1;
      m->b = 2;
      m->c = 3;
      NetworkManager::SendMessageFromClient(m);
    }
  });
  Input::RegisterKeyPressCallback(KeyCode::H, []() {
    if (NetworkManager::ClientIsConnected()) {
      if (NetworkingExample::despawnCounter >=
          NetworkingExample::spawnedEntities.size()) {
        return;
      }
      DespawnExample* m = reinterpret_cast<DespawnExample*>(
          NetworkManager::GenerateMessageFromClient("DSPN"));
      m->netId = NetworkingExample::despawnCounter++;
      NetworkManager::SendMessageFromClient(m);
    }
  });

  Input::RegisterKeyPressCallback(KeyCode::P, []() {
    if (NetworkManager::ClientIsConnected()) {
      HandleMessage* handleMessage = reinterpret_cast<HandleMessage*>(
          NetworkManager::GenerateMessageFromClient("HNDL"));
      handleMessage->handle = 0;
      NetworkManager::SendMessageFromClient(handleMessage);
    }
  });
  Input::RegisterKeyPressCallback(KeyCode::O, []() {
    if (NetworkManager::ClientIsConnected()) {
      HandleMessage* handleMessage = reinterpret_cast<HandleMessage*>(
          NetworkManager::GenerateMessageFromClient("HNDL"));
      handleMessage->handle = 1;
      NetworkManager::SendMessageFromClient(handleMessage);
    }
  });
  Input::RegisterMousePressCallback(MouseButtonCode::MOUSE_LEFT, []() {
    if (NetworkManager::ClientIsConnected()) {
      HandleMessage* handleMessage = reinterpret_cast<HandleMessage*>(
          NetworkManager::GenerateMessageFromClient("HNDL"));
      handleMessage->handle = 2;
      NetworkManager::SendMessageFromClient(handleMessage);
    }
  });
}
void NetworkingDemoEnd() {
  NetworkingExample::RegisterExampleMessageFunctions();
}

}  // namespace Isetta
