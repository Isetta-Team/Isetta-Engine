#include "Custom/FontExample.h"

#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"

namespace Isetta {
void FontExample::Start() {
  GUI::AddFontFromFile("../External/imgui/misc/fonts/Roboto-Medium.ttf", 16.0f);
  auto defaultFont = GUI::AddFontFromFile(
      "../External/imgui/misc/fonts/Roboto-Medium.ttf", 32.0f);
  font = GUI::AddFontFromFile("../External/imgui/misc/fonts/Roboto-Medium.ttf",
                              72.0f);
  GUI::AddDefaultFont(defaultFont);
}
void FontExample::GuiUpdate() {
  GUI::Text(RectTransform{{0, 0, 0, 0}, GUI::Pivot::Center, GUI::Pivot::Center},
            "Test", GUI::TextStyle{font});
}
}  // namespace Isetta
