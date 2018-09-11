/*
 * Copyright (c) 2018 Isetta
 */
#include "ModuleManager.h"
#include "Audio/Audio.h"
#include "Core\Window.h"

namespace Isetta {

void ModuleManager::StartUp() {
  audioModule = new AudioModule();
  audioModule->StartUp();

  windowModule = new WindowModule();
  windowModule->StartUp();
}

void ModuleManager::Update() {
  audioModule->Update();
  windowModule->Update();
}

void ModuleManager::ShutDown() {
  audioModule->ShutDown();
  windowModule->ShutDown();
  delete windowModule;
  delete audioModule;
}
}  // namespace Isetta