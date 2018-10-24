/*
 * Copyright (c) 2018 Isetta
 */

#include "Application.h"
#include "EngineLoop.h"

namespace Isetta {

void Application::Exit() { EngineLoop::Instance().Stop(); }

}  // namespace Isetta