/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/RectTransform.h"
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(Inspector, Component, true)
private:
std::string title;
bool isOpen = true;
RectTransform rectTransform{{30, 300, 350, 300}};

public:
class Transform* target;

Inspector(std::string title, bool isOpen,
          class Transform* const target = nullptr);
void GuiUpdate() override;

void Open();
END_COMPONENT(Inspector, Component)
}  // namespace Isetta
