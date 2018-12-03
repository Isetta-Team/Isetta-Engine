/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Graphics/RectTransform.h"
#include "Scene/Component.h"

namespace Isetta {
/**
 * @brief Inspector shows debug information of selected transform including:
 * transformational information (position/rotation/scale), components, entity
 * information
 *
 */
DEFINE_COMPONENT(Inspector, Component, true)
public:
/**
 * @brief Transform debug information is about
 *
 */
class Transform* target;

/**
 * @brief Construct a new Inspector object
 *
 * @param title of the window
 * @param isOpen whether starts open
 * @param target transformation to display information
 */
Inspector(std::string title, bool isOpen, class Transform* target = nullptr);
void GuiUpdate() override;

void Open();

private:
std::string title;
bool isOpen = true;
RectTransform rectTransform{{30, 300, 350, 300}};
DEFINE_COMPONENT_END(Inspector, Component)
}  // namespace Isetta
