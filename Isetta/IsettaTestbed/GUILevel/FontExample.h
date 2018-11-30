/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
BEGIN_COMPONENT(FontExample, Component, true)
private:
float fontSize;

public:
FontExample(float fontSize,
            const std::string_view fontName = "Fonts\\CONSOLA.TTF");

void GuiUpdate() override;
END_COMPONENT(FontExample, Component)
}  // namespace Isetta