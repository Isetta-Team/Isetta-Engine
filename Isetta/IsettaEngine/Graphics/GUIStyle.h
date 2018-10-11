/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/Color.h"
#include "Core/Math/Vector2.h"
#include "Graphics/GUI.h"

namespace Isetta {
ISETTA_API struct GUIStyle {
  float Alpha;                  // Global alpha applies to everything in ImGui.
  Math::Vector2 WindowPadding;  // Padding within a window.
  float WindowRounding;    // Radius of window corners rounding. Set to 0.0f
                           // to have rectangular windows.
  float WindowBorderSize;  // Thickness of border around windows. Generally
                           // set to 0.0f or 1.0f. (Other values are not
                           // well tested and more CPU/GPU costly).
  Math::Vector2 WindowMinSize;  // Minimum window size. This is a global
                                // setting. If
  // you want to constraint individual windows, use
  // SetNextWindowSizeConstraints().
  Math::Vector2 WindowTitleAlign;  // Alignment for title bar text. Defaults
                                   // to (0.0f,0.5f) for
                                   // left-aligned,vertically centered.
  float ChildRounding;    // Radius of child window corners rounding. Set to
                          // 0.0f to have rectangular windows.
  float ChildBorderSize;  // Thickness of border around child windows.
                          // Generally set to 0.0f or 1.0f. (Other values
                          // are not well tested and more CPU/GPU costly).
  float PopupRounding;    // Radius of popup window corners rounding. (Note
                          // that tooltip windows use WindowRounding)
  float PopupBorderSize;  // Thickness of border around popup/tooltip windows.
                          // Generally set to 0.0f or 1.0f. (Other values are
                          // not well tested and more CPU/GPU costly).
  Math::Vector2 FramePadding;  // Padding within a framed rectangle (used by
                               // most
                               // widgets).
  float FrameRounding;    // Radius of frame corners rounding. Set to 0.0f to
                          // have rectangular frame (used by most widgets).
  float FrameBorderSize;  // Thickness of border around frames. Generally
                          // set to 0.0f or 1.0f. (Other values are not well
                          // tested and more CPU/GPU costly).
  Math::Vector2 ItemSpacing;       // Horizontal and vertical spacing between
                                   // widgets/lines.
  Math::Vector2 ItemInnerSpacing;  // Horizontal and vertical spacing
                                   // between within
  // elements of a composed widget (e.g. a slider and
  // its label).
  Math::Vector2
      TouchExtraPadding;    // Expand reactive bounding box for touch-based
                            // system where touch position is not accurate
                            // enough. Unfortunately we don't sort widgets so
                            // priority on overlap will always be given to the
                            // first widget. So don't grow this too much!
  float IndentSpacing;      // Horizontal indentation when e.g. entering a tree
                            // node. Generally == (FontSize + FramePadding.x*2).
  float ColumnsMinSpacing;  // Minimum horizontal spacing between two columns.
  float ScrollbarSize;      // Width of the vertical scrollbar, Height of the
                            // horizontal scrollbar.
  float ScrollbarRounding;  // Radius of grab corners for scrollbar.
  float GrabMinSize;        // Minimum width/height of a grab box for
                            // slider/scrollbar.
  float GrabRounding;       // Radius of grabs corners rounding. Set to 0.0f to
                            // have rectangular slider grabs.
  Math::Vector2 ButtonTextAlign;  // Alignment of button text when button is
                                  // larger
                                  // than text. Defaults to (0.5f,0.5f) for
                                  // horizontally+vertically centered.
  Math::Vector2
      DisplayWindowPadding;  // Window position are clamped to be visible
                             // within the display area by at least this
                             // amount. Only applies to regular windows.
  Math::Vector2
      DisplaySafeAreaPadding;  // If you cannot see the edges of your screen
                               // (e.g. on a TV) increase the safe area
                               // padding. Apply to popups/tooltips as well
                               // regular windows. NB: Prefer configuring
                               // your TV sets correctly!
  float MouseCursorScale;      // Scale software rendered mouse cursor (when
                               // io.MouseDrawCursor is enabled). May be removed
                               // later.
  bool AntiAliasedLines;       // Enable anti-aliasing on lines/borders. Disable
                               // if you are really tight on CPU/GPU.
  bool AntiAliasedFill;        // Enable anti-aliasing on filled shapes (rounded
                               // rectangles, circles, etc.)
  float CurveTessellationTol;  // Tessellation tolerance when using
                               // PathBezierCurveTo() without a specific
                               // number of segments. Decrease for highly
                               // tessellated curves (higher quality, more
                               // polygons), increase to reduce quality.
  Color Colors[static_cast<int>(GUI::ColorStyles::COUNT)];  // TODO(JACOB)

  explicit GUIStyle(bool imGuiStyle = true);
};
}  // namespace Isetta
