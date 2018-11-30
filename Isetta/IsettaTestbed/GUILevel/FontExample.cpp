/*
 * Copyright (c) 2018 Isetta
 */
#include "FontExample.h"

namespace Isetta {
FontExample::FontExample(float fontSize, const std::string_view fontName)
    : fontSize{fontSize} {
  Font::AddFontFromFile(fontName.data(), 16.0f, "FontExampleName");
  Font::AddFontFromFile(fontName.data(), 32.0f, "FontExampleName");
  if (fontSize > 0)
    Font::AddFontFromFile(fontName.data(), fontSize, "FontExampleName");
  else
    fontSize = 16.0f;
}
void FontExample::GuiUpdate() {
  GUI::Text(
      RectTransform{{0, 100, 0, 0}, GUI::Pivot::Center, GUI::Pivot::Center},
      "Test", GUI::TextStyle{fontSize, "FontNameExample"});
}
}  // namespace Isetta
