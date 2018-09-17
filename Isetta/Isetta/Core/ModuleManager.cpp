/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/ModuleManager.h"
#include "Audio/AudioModule.h"
#include "Core/Memory/MemoryManager.h"
#include "Graphics/RenderModule.h"
#include "Graphics/Window.h"
#include "Input/InputModule.h"

namespace Isetta {
ModuleManager::ModuleManager() {
  memoryManager = new MemoryManager{};
  audioModule = new AudioModule{};
  windowModule = new WindowModule{};
  renderModule = new RenderModule{};
  inputModule = new InputModule{};
}
ModuleManager::~ModuleManager() {
  delete windowModule;
  delete audioModule;
  delete renderModule;
  delete inputModule;
  delete memoryManager;
}

void ModuleManager::StartUp() {
  memoryManager->StartUp();
  audioModule->StartUp();
  windowModule->StartUp();
  renderModule->StartUp(windowModule->winHandle);
  inputModule->StartUp(windowModule->winHandle);
}

void ModuleManager::SimulationUpdate(float deltaTime) {
  audioModule->Update(deltaTime);
  inputModule->Update(deltaTime);
  memoryManager->Update();
}

void ModuleManager::RenderUpdate(float deltaTime) {
  renderModule->Update(deltaTime);
  windowModule->Update(deltaTime);
}

void ModuleManager::ShutDown() {
  audioModule->ShutDown();
  renderModule->ShutDown();
  inputModule->ShutDown();
  windowModule->ShutDown();
  memoryManager->ShutDown();
}
}  // namespace Isetta
