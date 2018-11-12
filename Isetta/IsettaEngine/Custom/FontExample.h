/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(FontExample, Component, true)
private:
class Font* font;

public:
void Start() override;
void GuiUpdate() override;
END_COMPONENT(FontExample, Component)
}  // namespace Isetta