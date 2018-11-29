/*
 * Copyright (c) 2018 Isetta
 */
#include "GUIStyle.h"

#include "Font.h"
#include "GUI.h"
#include "GUIModule.h"
#include "imgui/imgui.h"

namespace Isetta {
GUIStyle::GUIStyle(bool imGuiStyle) {
  if (!imGuiStyle) return;
  GUIStyle style = GUI::GetStyle();
  Alpha = style.Alpha;
  WindowPadding = style.WindowPadding;
  WindowRounding = style.WindowRounding;
  WindowBorderSize = style.WindowBorderSize;
  WindowMinSize = style.WindowMinSize;
  WindowTitleAlign = style.WindowTitleAlign;
  ChildRounding = style.ChildRounding;
  ChildBorderSize = style.ChildBorderSize;
  PopupRounding = style.PopupRounding;
  PopupBorderSize = style.PopupBorderSize;
  FramePadding = style.FramePadding;
  FrameRounding = style.FrameRounding;
  FrameBorderSize = style.FrameBorderSize;
  ItemSpacing = style.ItemSpacing;
  ItemInnerSpacing = style.ItemInnerSpacing;
  TouchExtraPadding = style.TouchExtraPadding;
  IndentSpacing = style.IndentSpacing;
  ColumnsMinSpacing = style.ColumnsMinSpacing;
  ScrollbarSize = style.ScrollbarSize;
  ScrollbarRounding = style.ScrollbarRounding;
  GrabMinSize = style.GrabMinSize;
  GrabRounding = style.GrabRounding;
  ButtonTextAlign = style.ButtonTextAlign;
  DisplayWindowPadding = style.DisplayWindowPadding;
  DisplaySafeAreaPadding = style.DisplaySafeAreaPadding;
  MouseCursorScale = style.MouseCursorScale;
  AntiAliasedFill = style.AntiAliasedFill;
  CurveTessellationTol = style.CurveTessellationTol;
  for (int i = 0; i < static_cast<int>(GUI::ColorStyles::COUNT); ++i) {
    Colors[i] = style.Colors[i];
  }
}

GUI::ButtonStyle::ButtonStyle() {
  background = GetStyle().Colors[(int)ColorStyles::Button];
  hover = GetStyle().Colors[(int)ColorStyles::ButtonHovered];
  active = GetStyle().Colors[(int)ColorStyles::ButtonActive];
}
GUI::InputStyle::InputStyle() {
  background = GetStyle().Colors[(int)ColorStyles::FrameBg];
  hovered = GetStyle().Colors[(int)ColorStyles::FrameBgHovered];
  active = GetStyle().Colors[(int)ColorStyles::FrameBgActive];
  text = GetStyle().Colors[(int)ColorStyles::Text];
  font = Font::GetDefaultFont();
}
GUI::InputStyle::InputStyle(Font* const font) : font(font) {
  background = GetStyle().Colors[(int)ColorStyles::FrameBg];
  hovered = GetStyle().Colors[(int)ColorStyles::FrameBgHovered];
  active = GetStyle().Colors[(int)ColorStyles::FrameBgActive];
  text = GetStyle().Colors[(int)ColorStyles::Text];
}
GUI::InputStyle::InputStyle(const Color& background, const Color& hovered,
                            const Color& active, const Color& text)
    : background{background}, hovered{hovered}, active{active}, text{text} {
  font = Font::GetDefaultFont();
}
GUI::LabelStyle::LabelStyle() {
  text = GetStyle().Colors[(int)ColorStyles::Text];
  background = GetStyle().Colors[(int)ColorStyles::FrameBg];
  font = Font::GetDefaultFont();
}
GUI::LabelStyle::LabelStyle(Font* const font) : font{font} {
  text = GetStyle().Colors[(int)ColorStyles::Text];
  background = GetStyle().Colors[(int)ColorStyles::FrameBg];
}
GUI::LabelStyle::LabelStyle(const Color& text, const Color& background)
    : text{text}, background{background} {
  font = Font::GetDefaultFont();
}
GUI::ProgressBarStyle::ProgressBarStyle() {
  background = GetStyle().Colors[(int)ColorStyles::FrameBg];
  bar = GetStyle().Colors[(int)ColorStyles::PlotHistogram];
  overlayText = GetStyle().Colors[(int)ColorStyles::Text];
  hoverChange = false;
  hover = GetStyle().Colors[(int)ColorStyles::PlotHistogramHovered];
}
GUI::ProgressBarStyle::ProgressBarStyle(const Color& background,
                                        const Color& bar,
                                        const Color& overlayText)
    : background{background}, bar{bar}, overlayText{overlayText} {
  hoverChange = false;
  hover = GetStyle().Colors[(int)ColorStyles::PlotHistogramHovered];
}
GUI::ModalStyle::ModalStyle() {
  window = GetStyle().Colors[(int)ColorStyles::WindowBg];
  background = GetStyle().Colors[(int)ColorStyles::ModalWindowDimBg];
}
GUI::TextStyle::TextStyle() {
  text = GetStyle().Colors[(int)ColorStyles::Text];
  font = reinterpret_cast<Font*>(ImGui::GetIO().FontDefault);
}
GUI::TextStyle::TextStyle(float fontSize, const std::string_view& fontName) {
  text = GetStyle().Colors[(int)ColorStyles::Text];
  Font* loadFont = Font::GetFont(fontName, fontSize);
  if (loadFont)
    font = loadFont;
  else {
    LOG_WARNING(Debug::Channel::GUI,
                "GUI::TextStyle::TextStyle => Font could not be found: %s",
                fontName);
    font = reinterpret_cast<Font*>(ImGui::GetIO().FontDefault);
  }
}
GUI::TextStyle::TextStyle(Font* const font) : font{font} {
  text = GetStyle().Colors[(int)ColorStyles::Text];
}
GUI::TextStyle::TextStyle(const Color& text, float fontSize,
                          const std::string_view& fontName)
    : text{text} {
  Font* loadFont = Font::GetFont(fontName, fontSize);
  if (loadFont)
    font = loadFont;
  else {
    LOG_WARNING(Debug::Channel::GUI,
                "GUI::TextStyle::TextStyle => Font could not be found: %s",
                fontName);
    font = reinterpret_cast<Font*>(ImGui::GetIO().FontDefault);
  }
}
GUI::TextStyle::TextStyle(bool wrapped, bool disabled, const Color& text)
    : isWrapped{wrapped},
      isDisabled{disabled},
      /*isBulleted{b},*/ text{text} {
  font = reinterpret_cast<Font*>(ImGui::GetIO().FontDefault);
}
GUI::WindowStyle::WindowStyle() {
  background = GetStyle().Colors[(int)ColorStyles::WindowBg];
  constraints = Math::Rect{};
}
GUI::WindowStyle::WindowStyle(const Math::Rect& constraints)
    : constraints{constraints} {
  background = GetStyle().Colors[(int)ColorStyles::WindowBg];
}
}  // namespace Isetta
