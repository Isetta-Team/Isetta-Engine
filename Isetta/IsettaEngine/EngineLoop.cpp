/*
 * Copyright (c) 2018 Isetta
 */
#include "EngineLoop.h"

#include "Audio/AudioModule.h"
#include "Collisions/CollisionSolverModule.h"
#include "Collisions/CollisionsModule.h"
#include "Core/Memory/MemoryManager.h"
#include "Graphics/GUIModule.h"
#include "Graphics/RenderModule.h"
#include "Graphics/Window.h"
#include "Input/InputModule.h"
#include "Networking/NetworkingModule.h"

#include "Core/Config/Config.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Debug/Logger.h"
#include "Core/Filesystem.h"
#include "Core/Time/Clock.h"
#include "Events/Events.h"
#include "Input/Input.h"
#include "Input/KeyCode.h"
#include "Networking/NetworkManager.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

EngineLoop& EngineLoop::Instance() {
  static EngineLoop instance;
  return instance;
}

EngineLoop::EngineLoop() {
  memoryManager = new MemoryManager{};
  windowModule = new WindowModule{};
  renderModule = new RenderModule{};
  inputModule = new InputModule{};
  guiModule = new GUIModule{};
  collisionsModule = new CollisionsModule{};
  collisionSolverModule = new CollisionSolverModule{};
  audioModule = new AudioModule{};
  networkingModule = new NetworkingModule{};
  events = new Events{};
}
EngineLoop::~EngineLoop() {
  delete windowModule;
  delete renderModule;
  delete inputModule;
  delete guiModule;
  delete collisionsModule;
  delete collisionSolverModule;
  delete audioModule;
  delete networkingModule;
  delete events;
  memoryManager->ShutDown();
  delete memoryManager;
}

void EngineLoop::StartUp() {
  BROFILER_EVENT("Start Up");

  Logger::NewSession();
  Config::Instance().Read("config.cfg");
  if (Filesystem::Instance().FileExists("user.cfg")) {
    Config::Instance().Read("user.cfg");
  }

  intervalTime = 1.0 / Config::Instance().loopConfig.maxFps.GetVal();
  maxSimulationCount = Config::Instance().loopConfig.maxSimCount.GetVal();

  // Will be set to false when Application set it to isGameRunning
  isGameRunning = true;

  // Memory module must start before everything else
  memoryManager->StartUp();
  // Window module must start before things depend on it
  windowModule->StartUp();
  renderModule->StartUp(windowModule->winHandle);
  inputModule->StartUp(windowModule->winHandle);
  guiModule->StartUp(windowModule->winHandle);
  DebugDraw::StartUp();
  collisionsModule->StartUp();
  collisionSolverModule->StartUp();
  audioModule->StartUp();
  networkingModule->StartUp();
  events->StartUp();

  LevelManager::Instance().LoadLevel(CONFIG_VAL(levelConfig.startLevel));
  LevelManager::Instance().LoadLevel();

  StartGameClock();
}

void EngineLoop::Update() {
  BROFILER_FRAME("Main Thread");

  GetGameClock().UpdateTime();

  accumulateTime += GetGameClock().GetDeltaTime();

  for (int i = 0; i < maxSimulationCount && accumulateTime > intervalTime;
       ++i) {
    FixedUpdate(intervalTime);
    accumulateTime -= intervalTime;
  }

  VariableUpdate(GetGameClock().GetDeltaTime());
}

void EngineLoop::FixedUpdate(const float deltaTime) const {
  BROFILER_CATEGORY("Fixed Update", Profiler::Color::IndianRed);

  networkingModule->Update(deltaTime);
  collisionsModule->Update(deltaTime);
  collisionSolverModule->Update();
  LevelManager::Instance().loadedLevel->FixedUpdate();
}
void EngineLoop::VariableUpdate(const float deltaTime) const {
  BROFILER_CATEGORY("Variable Update", Profiler::Color::SteelBlue);

  inputModule->Update(deltaTime);
  LevelManager::Instance().loadedLevel->Update();
  Events::Instance().Update();
  LevelManager::Instance().loadedLevel->LateUpdate();
  audioModule->Update(deltaTime);
  renderModule->Update(deltaTime);
  DebugDraw::Update();
  guiModule->Update(deltaTime);
  windowModule->Update(deltaTime);
  memoryManager->Update();

  if (LevelManager::Instance().LoadLevel()) {
    inputModule->Clear();
    audioModule->UnloadLevel();
  }
}

void EngineLoop::ShutDown() {
  BROFILER_EVENT("Shut Down");

  LevelManager::Instance().UnloadLevel();
  events->ShutDown();
  networkingModule->ShutDown();
  audioModule->ShutDown();
  collisionsModule->ShutDown();
  collisionSolverModule->ShutDown();
  DebugDraw::ShutDown();
  guiModule->ShutDown();
  inputModule->ShutDown();
  renderModule->ShutDown();
  windowModule->ShutDown();
  Logger::ShutDown();
}

void EngineLoop::StartGameClock() const { GetGameClock(); }

void EngineLoop::Run() {
  ASSERT(!isGameRunning);
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
}  // namespace Isetta
