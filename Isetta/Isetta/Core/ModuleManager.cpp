#include "ModuleManager.h"
#include "Audio/Audio.h"

namespace Isetta {

void ModuleManager::StartUp() {
  audioModule = new AudioModule();
  audioModule->StartUp();
  
  
}

void ModuleManager::Update() {
  audioModule->Update();
}

void ModuleManager::ShutDown() {
  audioModule->ShutDown();
}

}  // namespace Isetta