/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/GUIStyle.h"

#include "GUI.h"

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
  for (int i = 0; i < static_cast<int>(GUI::ColorStyles::COUNT); i++) {
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
}
GUI::LabelStyle::LabelStyle() {
  text = GetStyle().Colors[(int)ColorStyles::Text];
  background = GetStyle().Colors[(int)ColorStyles::FrameBg];
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
GUI::TextStyle::TextStyle() {
  text = GetStyle().Colors[(int)ColorStyles::Text];
}

}  // namespace Isetta
