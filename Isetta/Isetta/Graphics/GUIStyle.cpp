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
  for (int i = 0; i < static_cast<int>(GUIColorStyles::COUNT); i++) {
    Colors[i] = style.Colors[i];
  }
}

GUIStyle::ButtonStyle::ButtonStyle() {
  background = GUI::GetStyle().Colors[(int)GUIColorStyles::Button];
  hover = GUI::GetStyle().Colors[(int)GUIColorStyles::ButtonHovered];
  active = GUI::GetStyle().Colors[(int)GUIColorStyles::ButtonActive];
}

GUIStyle::InputStyle::InputStyle() {
  background = GUI::GetStyle().Colors[(int)GUIColorStyles::FrameBg];
  hovered = GUI::GetStyle().Colors[(int)GUIColorStyles::FrameBgHovered];
  active = GUI::GetStyle().Colors[(int)GUIColorStyles::FrameBgActive];
  text = GUI::GetStyle().Colors[(int)GUIColorStyles::Text];
}
GUIStyle::ProgressBarStyle::ProgressBarStyle() {
  background = GUI::GetStyle().Colors[(int)GUIColorStyles::FrameBg];
  bar = GUI::GetStyle().Colors[(int)GUIColorStyles::PlotHistogram];
  overlayText = GUI::GetStyle().Colors[(int)GUIColorStyles::Text];
  hoverChange = false;
  hover = GUI::GetStyle().Colors[(int)GUIColorStyles::PlotHistogramHovered];
}
GUIStyle::ProgressBarStyle::ProgressBarStyle(const Color& background,
                                             const Color& bar,
                                             const Color& overlayText)
    : background{background}, bar{bar}, overlayText{overlayText} {
  hoverChange = false;
  hover = GUI::GetStyle().Colors[(int)GUIColorStyles::PlotHistogramHovered];
}
GUIStyle::TextStyle::TextStyle() {
  text = GUI::GetStyle().Colors[(int)GUIColorStyles::Text];
}

}  // namespace Isetta
