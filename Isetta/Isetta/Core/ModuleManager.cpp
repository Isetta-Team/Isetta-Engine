/*
 * Copyright (c) 2018 Isetta
 */
#include "ModuleManager.h"
#include "Audio/Audio.h"
#include "Core/Graphics/Render.h"
#include "Core/Input/Input.h"

namespace Isetta {

void ModuleManager::StartUp() {
  audioModule = new AudioModule();
  audioModule->StartUp();

  renderModule = new RenderModule();
  renderModule->StartUp();

  inputModule = new InputModule();
  inputModule->StartUp();
}

void ModuleManager::Update() {
  audioModule->Update();
  renderModule->Update();
}

void ModuleManager::ShutDown() {
  audioModule->ShutDown();

  renderModule->ShutDown();
}
}  // namespace Isetta