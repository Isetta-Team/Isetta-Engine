/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/RectTransform.h"
#include "Scene/Component.h"

namespace Isetta {
/**
 * @brief Hierarchy displays the scene hierarchy (parents/children) of level,
 * can select entities to open inspector (if one is attached)
 *
 */
DEFINE_COMPONENT(Hierarchy, Component, true)
private:
std::string title;
bool isOpen = true;
RectTransform rectTransform{{30, 30, 160, 300}};

public:
/**
 * @brief Inspector to open on entity select
 *
 */
class Inspector* inspector;

/**
 * @brief Hierarchy of the scene
 *
 * @param title of the window
 * @param isOpen whether the window starts as open
 * @param inspector to attach when entity selected
 */
Hierarchy(std::string title, bool isOpen, class Inspector* inspector = nullptr);
void GuiUpdate() override;

void Open();
DEFINE_COMPONENT_END(Hierarchy, Component)
}  // namespace Isetta
