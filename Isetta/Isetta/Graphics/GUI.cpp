/*
 * Copyright (c) 2018 Isetta
 */
#include "GUI.h"

#include "Graphics/GUIModule.h"
#include "imgui.h"

namespace Isetta {
GUIModule* GUI::guiModule{nullptr};

void GUI::OnUpdate(const Action<>& callback) { guiModule->OnUpdate(callback); }
}  // namespace Isetta
