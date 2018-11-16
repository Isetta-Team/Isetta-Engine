#include "Custom/FontExample.h"

#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "imgui/imgui.h"

namespace Isetta {
FontExample::FontExample(float fontSize, const std::string_view fontName)
    : fontSize{fontSize} {
  font = GUI::AddFontFromFile(fontName.data(), 16.0f, "FontExampleName");
  auto defaultFont =
      GUI::AddFontFromFile(fontName.data(), 32.0f, "FontExampleName");
  if (fontSize > 0)
    font = GUI::AddFontFromFile(fontName.data(), fontSize, "FontExampleName");
  else
    fontSize = 16.0f;
  GUI::AddDefaultFont(defaultFont);
}
void FontExample::GuiUpdate() {
  GUI::Text(
      RectTransform{{0, 100, 0, 0}, GUI::Pivot::Center, GUI::Pivot::Center},
      "Test", GUI::TextStyle{fontSize, "FontNameExample"});
}
}  // namespace Isetta
