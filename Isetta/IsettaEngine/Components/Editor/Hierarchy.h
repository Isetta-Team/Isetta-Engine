/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/RectTransform.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(Hierarchy, Component, true)
private:
std::string title;
bool isOpen = true;
RectTransform rectTransform{{30, 30, 140, 300}};

public:
class Inspector* inspector;

Hierarchy(std::string title, bool isOpen, class Inspector* inspector = nullptr);
void GuiUpdate() override;

void Open();
END_COMPONENT(Hierarchy, Component)
}  // namespace Isetta
