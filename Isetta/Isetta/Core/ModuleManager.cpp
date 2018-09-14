/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/ModuleManager.h"
#include "Audio/AudioModule.h"
#include "Graphics/RenderModule.h"
#include "Graphics/Window.h"
#include "Input/InputModule.h"

namespace Isetta {
ModuleManager::ModuleManager() {
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
}
void ModuleManager::StartUp() {
  audioModule->StartUp();
  windowModule->StartUp();
  renderModule->StartUp(windowModule->winHandle);
  inputModule->StartUp(windowModule->winHandle);
}

void ModuleManager::Update(float deltaTime) {
  audioModule->Update(deltaTime);
  inputModule->Update(deltaTime);
  renderModule->Update(deltaTime);
  windowModule->Update(deltaTime);
}

void ModuleManager::ShutDown() {
  audioModule->ShutDown();
  renderModule->ShutDown();
  inputModule->ShutDown();
  windowModule->ShutDown();
}
}  // namespace Isetta
