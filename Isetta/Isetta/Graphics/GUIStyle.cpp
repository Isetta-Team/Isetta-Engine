/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/GUIStyle.h"

#include "imgui.h"

namespace Isetta {
GUIStyle::GUIStyle(const ImGuiStyle& style) {
  Alpha = style.Alpha;
  WindowPadding =
      Isetta::Math::Vector2(style.WindowPadding.x, style.WindowPadding.y);
  WindowRounding = style.WindowRounding;
  WindowBorderSize = style.WindowBorderSize;
  WindowMinSize =
      Isetta::Math::Vector2(style.WindowMinSize.x, style.WindowMinSize.y);
  WindowTitleAlign =
      Isetta::Math::Vector2(style.WindowTitleAlign.x, style.WindowTitleAlign.y);
  ChildRounding = style.ChildRounding;
  ChildBorderSize = style.ChildBorderSize;
  PopupRounding = style.PopupRounding;
  PopupBorderSize = style.PopupBorderSize;
  FramePadding = Isetta::Math::Vector2(style.FramePadding.x, FramePadding.y);
  FrameRounding = style.FrameRounding;
  FrameBorderSize = style.FrameBorderSize;
  ItemSpacing = Isetta::Math::Vector2(style.ItemSpacing.x, style.ItemSpacing.y);
  ItemInnerSpacing =
      Isetta::Math::Vector2(style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);
  TouchExtraPadding = Isetta::Math::Vector2(style.TouchExtraPadding.x,
                                            style.TouchExtraPadding.y);
  IndentSpacing = style.IndentSpacing;
  ColumnsMinSpacing = style.ColumnsMinSpacing;
  ScrollbarSize = style.ScrollbarSize;
  ScrollbarRounding = style.ScrollbarRounding;
  GrabMinSize = style.GrabMinSize;
  GrabRounding = style.GrabRounding;
  ButtonTextAlign =
      Isetta::Math::Vector2(style.ButtonTextAlign.x, style.ButtonTextAlign.y);
  DisplayWindowPadding = Isetta::Math::Vector2(style.DisplayWindowPadding.x,
                                               style.DisplayWindowPadding.y);
  DisplaySafeAreaPadding = Isetta::Math::Vector2(
      style.DisplaySafeAreaPadding.x, style.DisplaySafeAreaPadding.y);
  MouseCursorScale = style.MouseCursorScale;
  AntiAliasedFill = style.AntiAliasedFill;
  CurveTessellationTol = style.CurveTessellationTol;
  for (int i = 0; i < ImGuiCol_COUNT; i++) {
    Colors[i] = Color(style.Colors[i]);
  }
}  // namespace Isetta
}  // namespace Isetta