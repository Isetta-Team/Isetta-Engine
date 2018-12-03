/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
DEFINE_COMPONENT(FontExample, Component, true)
private:
float fontSize;

public:
FontExample(float fontSize,
            const std::string_view fontName = "Fonts\\CONSOLA.TTF");

void GuiUpdate() override;
DEFINE_COMPONENT_END(FontExample, Component)
}  // namespace Isetta