/*
 * Copyright (c) 2018 Isetta
 */

#include "Application.h"
#include "EngineLoop.h"

namespace Isetta {

void Application::Start() {
  if (EngineLoop::Instance().isGameRunning) {
    throw std::exception(
        "Cannot start engine after it has already been started!");
  }
  EngineLoop::Instance().Run();
}

void Application::Exit() { EngineLoop::Instance().isGameRunning = false; }

}  // namespace Isetta