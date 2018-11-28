/*
 * Copyright (c) 2018 Isetta
 */
#include "FontExample.h"

#include "Graphics/Font.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"

namespace Isetta {
FontExample::FontExample(float fontSize, const std::string_view fontName)
    : fontSize{fontSize} {
  font = Font::AddFontFromFile(fontName.data(), 16.0f, "FontExampleName");
  auto defaultFont =
      Font::AddFontFromFile(fontName.data(), 32.0f, "FontExampleName");
  if (fontSize > 0)
    font = Font::AddFontFromFile(fontName.data(), fontSize, "FontExampleName");
  else
    fontSize = 16.0f;
  Font::AddDefaultFont(defaultFont);
}
void FontExample::GuiUpdate() {
  GUI::Text(
      RectTransform{{0, 100, 0, 0}, GUI::Pivot::Center, GUI::Pivot::Center},
      "Test", GUI::TextStyle{fontSize, "FontNameExample"});
}
}  // namespace Isetta
