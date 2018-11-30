/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/RectTransform.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(Hierarchy, Component, true)
private:
std::string title;
bool isOpen = true;
RectTransform rectTransform{{30, 30, 160, 300}};

public:
class Inspector* inspector;

Hierarchy(std::string title, bool isOpen, class Inspector* inspector = nullptr);
void GuiUpdate() override;

void Open();
DEFINE_COMPONENT_END(Hierarchy, Component)
}  // namespace Isetta
