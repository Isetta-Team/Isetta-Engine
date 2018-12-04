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
#include "Graphics/WindowModule.h"
#include "Input/InputModule.h"
#include "Networking/NetworkingModule.h"

#include "Core/Config/Config.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Debug/Logger.h"
#include "Core/Filesystem.h"
#include "Core/Time/Clock.h"
#include "Events/Events.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

EngineLoop& EngineLoop::Instance() {
  static EngineLoop instance;
  return instance;
}

/**
 * @brief:
 * Initialize the logger for debugging
 * Read the configuration files
 * Start memory manager 1st
 * Ordering of the others isn't relevant
 *
 */
EngineLoop::EngineLoop() {
  Logger::NewSession();
  Config::Instance().Read("config.cfg");
  if (Filesystem::Instance().FileExists("user.cfg")) {
    Config::Instance().Read("user.cfg");
  }

  // Memory manager must start before everything else
  memoryManager = new MemoryManager{};
  windowModule = MemoryManager::NewOnStack<WindowModule>();
  renderModule = MemoryManager::NewOnStack<RenderModule>();
  inputModule = MemoryManager::NewOnStack<InputModule>();
  guiModule = MemoryManager::NewOnStack<GUIModule>();
  collisionsModule = MemoryManager::NewOnStack<CollisionsModule>();
  collisionSolverModule = MemoryManager::NewOnStack<CollisionSolverModule>();
  audioModule = MemoryManager::NewOnStack<AudioModule>();
  networkingModule = MemoryManager::NewOnStack<NetworkingModule>();
  events = MemoryManager::NewOnStack<Events>();
}

/**
 * @brief:
 * Delete memory manager last
 * Ordering of others isn't relevant
 *
 */
EngineLoop::~EngineLoop() {
  windowModule->~WindowModule();
  renderModule->~RenderModule();
  inputModule->~InputModule();
  guiModule->~GUIModule();
  collisionsModule->~CollisionsModule();
  collisionSolverModule->~CollisionSolverModule();
  audioModule->~AudioModule();
  networkingModule->~NetworkingModule();
  events->~Events();
  delete memoryManager;
}

/**
 * @brief: order that matters
 *  Window before Render/Input/Gui
 *  Level after all modules
 *
 */
void EngineLoop::StartUp() {
  BROFILER_EVENT("Start Up");

  intervalTime = 1.0 / Config::Instance().loopConfig.maxFps.GetVal();
  maxSimulationCount = Config::Instance().loopConfig.maxSimCount.GetVal();

  // Will be set to false when Application set it to isGameRunning
  isGameRunning = true;

  // Window module must start before things depend on it
  windowModule->StartUp();
  renderModule->StartUp(windowModule->winHandle);
  inputModule->StartUp(windowModule->winHandle);
  guiModule->StartUp(windowModule->winHandle);
#ifdef _EDITOR
  DebugDraw::StartUp();
#endif
  collisionsModule->StartUp();
  collisionSolverModule->StartUp();
  audioModule->StartUp();
  networkingModule->StartUp();
  events->StartUp();

  // Set which level to load
  LevelManager::Instance().LoadLevel(CONFIG_VAL(levelConfig.startLevel));
  // Actual perform the load
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

/**
 * @brief: order that matters
 * Level FixedUpdate must be last
 * CollisionSolver must be sandwiched between Collision Update and LateUpdate
 *
 */
void EngineLoop::FixedUpdate(const float deltaTime) const {
  BROFILER_CATEGORY("Fixed Update", Profiler::Color::IndianRed);

  networkingModule->Update(deltaTime);
  collisionsModule->Update(deltaTime);
  collisionSolverModule->Update();
  collisionsModule->LateUpdate(deltaTime);
  LevelManager::Instance().loadedLevel->FixedUpdate();
}

/**
 * @brief: order that matters
 * Input before Level Update
 * Event after Level Update
 * Level LateUpdate after Event
 * Audio after all Level Updates
 * Render after all Level
 * Render before DebugDraw/GUI/Window
 * Window after Render/DebugDraw/GUI/Window
 * Memory last
 * Load level after frame - more of a decision, could possibly (not probable)
 * work in middle of frame
 *
 */
void EngineLoop::VariableUpdate(const float deltaTime) const {
  BROFILER_CATEGORY("Variable Update", Profiler::Color::SteelBlue);

  inputModule->Update(deltaTime);
  LevelManager::Instance().loadedLevel->Update();
  Events::Instance().Update();
  LevelManager::Instance().loadedLevel->LateUpdate();
  audioModule->Update(deltaTime);
  renderModule->Update(deltaTime);
#ifdef _EDITOR
  DebugDraw::Update();
#endif
  guiModule->Update(deltaTime);
  windowModule->Update(deltaTime);
  memoryManager->Update();

  if (LevelManager::Instance().pendingLoadLevel) {
    LevelManager::Instance().UnloadLevel();
    inputModule->Clear();
    audioModule->UnloadLevel();
    DebugDraw::Clear();
    Events::Instance().Clear();
    LevelManager::Instance().LoadLevel();
  }
}

/**
 * @brief: order that matters
 * Unload level before anything else
 * Shut down in reverse order of Startup
 *
 */
void EngineLoop::ShutDown() {
  BROFILER_EVENT("Shut Down");

  LevelManager::Instance().UnloadLevel();
  events->ShutDown();
  networkingModule->ShutDown();
  audioModule->ShutDown();
  collisionsModule->ShutDown();
  collisionSolverModule->ShutDown();
#ifdef _EDITOR
  DebugDraw::ShutDown();
#endif
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
