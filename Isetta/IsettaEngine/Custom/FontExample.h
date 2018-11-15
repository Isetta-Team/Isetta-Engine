/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Scene/Component.h"

namespace Isetta {
BEGIN_COMPONENT(FontExample, Component, true)
private:
class Font* font;
float fontSize;

public:
FontExample(float fontSize = 0,
            const std::string_view fontName =
                "../External/imgui/misc/fonts/Roboto-Medium.ttf");

void GuiUpdate() override;
END_COMPONENT(FontExample, Component)
}  // namespace Isetta