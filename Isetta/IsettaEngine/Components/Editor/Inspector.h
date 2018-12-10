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
DEFINE_COMPONENT(Inspector, Component, false)
#ifdef _EDITOR
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
Inspector(std::string title, bool isOpen, bool isStatic = false,
          class Transform* target = nullptr);
void GuiUpdate() override;

void Open();
bool IsSelected(const Transform* const transform) const;
void SetAsInstance(bool isStatic = true);

static const Inspector* Instance();

private:
std::string title;
bool isOpen = true;
static Inspector* instance;
RectTransform rectTransform{{30, 300, 350, 300}};
#endif
DEFINE_COMPONENT_END(Inspector, Component)
}  // namespace Isetta
