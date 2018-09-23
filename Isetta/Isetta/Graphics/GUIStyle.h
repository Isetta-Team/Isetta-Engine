/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector4.h"

namespace Isetta {
enum class GUIWindowFlags : U32 {
  None = 0,
  NoTitleBar = 1 << 0,   // Disable title-bar
  NoResize = 1 << 1,     // Disable user resizing with the lower-right grip
  NoMove = 1 << 2,       // Disable user moving the window
  NoScrollbar = 1 << 3,  // Disable scrollbars (window can still scroll with
                         // mouse or programatically)
  NoScrollWithMouse =
      1 << 4,  // Disable user vertically scrolling with mouse wheel. On child
               // window, mouse wheel will be forwarded to the parent unless
               // NoScrollbar is also set.
  NoCollapse =
      1 << 5,  // Disable user collapsing window by double-clicking on it
  AlwaysAutoResize = 1 << 6,  // Resize every window to its content every frame
  NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
  NoInputs = 1 << 9,          // Disable catching mouse or keyboard
                              // inputs, hovering test with pass through.
  MenuBar = 1 << 10,          // Has a menu-bar
  HorizontalScrollbar =
      1 << 11,  // Allow horizontal scrollbar to appear (off by default). You
                // may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior
                // to calling Begin() to specify width. Read code in imgui_demo
                // in the "Horizontal Scrolling" section.
  NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from
                                 // hidden to visible state
  NoBringToFrontOnFocus =
      1 << 13,  // Disable bringing window to front when taking focus (e.g.
                // clicking on it or programatically giving it focus)
  AlwaysVerticalScrollbar =
      1
      << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
  AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar
                                        // (even if ContentSize.x < Size.x)
  AlwaysUseWindowPadding =
      1 << 16,  // Ensure child windows without border uses style.WindowPadding
                // (ignored by default for non-bordered child windows, because
                // more convenient)
  NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
  NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard
                         // navigation (e.g. skipped by CTRL+TAB)
  NoNav = NoNavInputs | NoNavFocus,

  // [Internal]
  NavFlattened = 1 << 23,  // [BETA] Allow gamepad/keyboard navigation to cross
                           // over parent border to this child (only use on
                           // child that have no scrolling!)
  ChildWindow = 1 << 24,   // Don't use! For internal use by BeginChild()
  Tooltip = 1 << 25,       // Don't use! For internal use by BeginTooltip()
  Popup = 1 << 26,         // Don't use! For internal use by BeginPopup()
  Modal = 1 << 27,         // Don't use! For internal use by BeginPopupModal()
  ChildMenu = 1 << 28      // Don't use! For internal use by BeginMenu()
};

enum class TreeNodeFlags : U16 {
  None = 0,
  Selected = 1 << 0,  // Draw as selected
  Framed = 1 << 1,    // Full colored frame (e.g. for CollapsingHeader)
  AllowItemOverlap =
      1 << 2,  // Hit testing to allow subsequent widgets to overlap this one
  NoTreePushOnOpen =
      1 << 3,  // Don't do a TreePush() when open (e.g. for CollapsingHeader) =
               // no extra indent nor pushing on ID stack
  NoAutoOpenOnLog =
      1 << 4,  // Don't automatically and temporarily open node when Logging is
               // active (by default logging will automatically open tree nodes)
  DefaultOpen = 1 << 5,        // Default node to be open
  OpenOnDoubleClick = 1 << 6,  // Need double-click to open node
  OpenOnArrow = 1 << 7,        // Only open when clicking on the arrow part. If
                               // OpenOnDoubleClick is also set, single-click
                               // arrow or double-click all box to open.
  Leaf =
      1 << 8,  // No collapsing, no arrow (use as a convenience for leaf nodes).
  Bullet = 1 << 9,  // Display a bullet instead of arrow
  FramePadding =
      1 << 10,  // Use FramePadding (even for an unframed text node) to
                // vertically align text baseline to regular widget height.
                // Equivalent to calling AlignTextToFramePadding().
  // SpanAllAvailWidth  = 1 << 11,  // FIXME: TODO: Extend
  // hit box horizontally even if not framed NoScrollOnOpen =
  // 1 << 12,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got
  // just open and contents is not visible
  NavLeftJumpsBackHere =
      1
      << 13,  // (WIP) Nav: left direction may move to this TreeNode() from any
              // of its child (items submitted between TreeNode and TreePop)
  CollapsingHeader = Framed | NoTreePushOnOpen | NoAutoOpenOnLog
};

enum class GUIColorStyles {
  Text,
  TextDisabled,
  WindowBg,  // Background of normal windows
  ChildBg,   // Background of child windows
  PopupBg,   // Background of popups, menus, tooltips windows
  Border,
  BorderShadow,
  FrameBg,  // Background of checkbox, radio button, plot, slider, text
            // input
  FrameBgHovered,
  FrameBgActive,
  TitleBg,
  TitleBgActive,
  TitleBgCollapsed,
  MenuBarBg,
  ScrollbarBg,
  ScrollbarGrab,
  ScrollbarGrabHovered,
  ScrollbarGrabActive,
  CheckMark,
  SliderGrab,
  SliderGrabActive,
  Button,
  ButtonHovered,
  ButtonActive,
  Header,
  HeaderHovered,
  HeaderActive,
  Separator,
  SeparatorHovered,
  SeparatorActive,
  ResizeGrip,
  ResizeGripHovered,
  ResizeGripActive,
  PlotLines,
  PlotLinesHovered,
  PlotHistogram,
  PlotHistogramHovered,
  TextSelectedBg,
  DragDropTarget,
  NavHighlight,           // Gamepad/keyboard: current highlighted item
  NavWindowingHighlight,  // Highlight window when using CTRL+TAB
  NavWindowingDimBg,      // Darken/colorize entire screen behind the
                          // CTRL+TAB window list, when active
  ModalWindowDimBg,       // Darken/colorize entire screen behind a modal
                          // window, when one is active
  COUNT
};

enum class InputTextFlags : U32 {
  None = 0,
  CharsDecimal = 1 << 0,      // Allow 0123456789.+-*/
  CharsHexadecimal = 1 << 1,  // Allow 0123456789ABCDEFabcdef
  CharsUppercase = 1 << 2,    // Turn a..z into A..Z
  CharsNoBlank = 1 << 3,      // Filter out spaces, tabs
  AutoSelectAll = 1 << 4,  // Select entire text when first taking mouse focus
  EnterReturnsTrue = 1 << 5,  // Return 'true' when Enter is pressed (as
                              // opposed to when the value was modified)
  CallbackCompletion =
      1 << 6,  // Call user function on pressing TAB (for completion handling)
  CallbackHistory = 1 << 7,  // Call user function on pressing Up/Down arrows
                             // (for history handling)
  CallbackAlways = 1 << 8,   // Call user function every time. User code may
                             // query cursor position, modify text buffer.
  CallbackCharFilter =
      1 << 9,  // Call user function to filter character. Modify
               // data->EventChar to replace/filter input, or return 1 in
               // callback to discard character.
  AllowTabInput =
      1 << 10,  // Pressing TAB input a '\t' character into the text field
  CtrlEnterForNewLine =
      1 << 11,  // In multi-line mode, unfocus with Enter, add new line with
                // Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter,
                // add line with Enter).
  NoHorizontalScroll = 1 << 12,  // Disable following the cursor horizontally
  AlwaysInsertMode = 1 << 13,    // Insert mode
  ReadOnly = 1 << 14,            // Read-only mode
  Password = 1 << 15,            // Password mode, display all characters as '*'
  NoUndoRedo =
      1 << 16,  // Disable undo/redo. Note that input text owns the text data
                // while active, if you want to provide your own undo/redo
                // stack you need e.g. to call ClearActiveID().
  CharsScientific =
      1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
  CallbackResize =
      1 << 18,  // Allow buffer capacity resize + notify when the string wants
                // to be resized (for string types which hold a cache of their
                // Size) (see misc/stl/imgui_stl.h for an example of using this)
  // [Internal]
  Multiline = 1 << 20  // For internal use by InputTextMultiline()
};

enum class ColorEditFlags : U32 {
  None = 0,
  NoAlpha = 1 << 1,   //              // ColorEdit, ColorPicker, ColorButton:
                      //              ignore Alpha component (read 3 components
                      //              from the input pointer).
  NoPicker = 1 << 2,  //              // ColorEdit: disable picker when clicking
                      //              on colored square.
  NoOptions =
      1 << 3,  //              // ColorEdit: disable toggling options menu when
               //              right-clicking on inputs/small preview.
  NoSmallPreview =
      1 << 4,  //              // ColorEdit, ColorPicker: disable colored square
               //              preview next to the inputs. (e.g. to show only
               //              the inputs)
  NoInputs = 1 << 5,  //              // ColorEdit, ColorPicker: disable inputs
                      //              sliders/text widgets (e.g. to show only
                      //              the small preview colored square).
  NoTooltip =
      1 << 6,  //              // ColorEdit, ColorPicker, ColorButton: disable
               //              tooltip when hovering the preview.
  NoLabel = 1 << 7,  //              // ColorEdit, ColorPicker: disable display
                     //              of inline text label (the label is still
                     //              forwarded to the tooltip and picker).
  NoSidePreview =
      1 << 8,  //              // ColorPicker: disable bigger color preview on
               //              right side of the picker, use small colored
               //              square preview instead.
  NoDragDrop =
      1 << 9,  //              // ColorEdit: disable drag and drop target.
               //              ColorButton: disable drag and drop source.

  // User Options (right-click on widget to change some of them). You can set
  // application defaults using SetColorEditOptions(). The idea is that you
  // probably don't want to override them in most of your calls, let the user
  // choose and/or call SetColorEditOptions() during startup.
  AlphaBar = 1 << 16,  //              // ColorEdit, ColorPicker: show vertical
                       //              alpha bar/gradient in picker.
  AlphaPreview =
      1 << 17,  //              // ColorEdit, ColorPicker, ColorButton: display
                //              preview as a transparent color over a
                //              checkerboard, instead of opaque.
  AlphaPreviewHalf =
      1 << 18,  //              // ColorEdit, ColorPicker, ColorButton: display
                //              half opaque / half checkerboard, instead of
                //              opaque.
  HDR = 1 << 19,    //              // (WIP) ColorEdit: Currently only disable
                    //              0.0f..1.0f limits in RGBA edition (note: you
                    //              probably want to use Float
                    //              flag as well).
  RGB = 1 << 20,    // [Inputs]     // ColorEdit: choose one among RGB/HSV/HEX.
                    // ColorPicker: choose any combination using RGB/HSV/HEX.
  HSV = 1 << 21,    // [Inputs]     // "
  HEX = 1 << 22,    // [Inputs]     // "
  Uint8 = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton:
                    // _display_ values formatted as 0..255.
  Float = 1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton:
                    // _display_ values formatted as 0.0f..1.0f floats instead
                    // of 0..255 integers. No round-trip of value via integers.
  PickerHueBar = 1 << 25,    // [PickerMode] // ColorPicker: bar for Hue,
                             // rectangle for Sat/Value.
  PickerHueWheel = 1 << 26,  // [PickerMode] // ColorPicker: wheel for Hue,
                             // triangle for Sat/Value.

  // [Internal] Masks
  InputsMask = RGB | HSV | HEX,
  DataTypeMask = Uint8 | Float,
  PickerMask = PickerHueWheel | PickerHueBar,
  OptionsDefault =
      Uint8 | RGB |
      PickerHueBar  // Change application default using SetColorEditOptions()
};

enum class GUIStyleParameter {
  Alpha,              // float     Alpha
  WindowPadding,      // ImVec2    WindowPadding
  WindowRounding,     // float     WindowRounding
  WindowBorderSize,   // float     WindowBorderSize
  WindowMinSize,      // ImVec2    WindowMinSize
  WindowTitleAlign,   // ImVec2    WindowTitleAlign
  ChildRounding,      // float     ChildRounding
  ChildBorderSize,    // float     ChildBorderSize
  PopupRounding,      // float     PopupRounding
  PopupBorderSize,    // float     PopupBorderSize
  FramePadding,       // ImVec2    FramePadding
  FrameRounding,      // float     FrameRounding
  FrameBorderSize,    // float     FrameBorderSize
  ItemSpacing,        // ImVec2    ItemSpacing
  ItemInnerSpacing,   // ImVec2    ItemInnerSpacing
  IndentSpacing,      // float     IndentSpacing
  ScrollbarSize,      // float     ScrollbarSize
  ScrollbarRounding,  // float     ScrollbarRounding
  GrabMinSize,        // float     GrabMinSize
  GrabRounding,       // float     GrabRounding
  ButtonTextAlign,    // ImVec2    ButtonTextAlign
  COUNT
};

enum class DrawCornerFlags : U8 {
  TopLeft = 1 << 0,             // 0x1
  TopRight = 1 << 1,            // 0x2
  BotLeft = 1 << 2,             // 0x4
  BotRight = 1 << 3,            // 0x8
  Top = TopLeft | TopRight,     // 0x3
  Bot = BotLeft | BotRight,     // 0xC
  Left = TopLeft | BotLeft,     // 0x5
  Right = TopRight | BotRight,  // 0xA
  All = 0xF
};

// TODO(Jacob) fix styling into class probably
struct GUIStyle {
  float Alpha;                  // Global alpha applies to everything in ImGui.
  Math::Vector2 WindowPadding;  // Padding within a window.
  float WindowRounding;    // Radius of window corners rounding. Set to 0.0f to
                           // have rectangular windows.
  float WindowBorderSize;  // Thickness of border around windows. Generally set
                           // to 0.0f or 1.0f. (Other values are not well tested
                           // and more CPU/GPU costly).
  Math::Vector2 WindowMinSize;  // Minimum window size. This is a global
                                // setting. If
  // you want to constraint individual windows, use
  // SetNextWindowSizeConstraints().
  Math::Vector2
      WindowTitleAlign;  // Alignment for title bar text. Defaults to
                         // (0.0f,0.5f) for left-aligned,vertically centered.
  float ChildRounding;   // Radius of child window corners rounding. Set to 0.0f
                         // to have rectangular windows.
  float ChildBorderSize;  // Thickness of border around child windows. Generally
                          // set to 0.0f or 1.0f. (Other values are not well
                          // tested and more CPU/GPU costly).
  float PopupRounding;    // Radius of popup window corners rounding. (Note that
                          // tooltip windows use WindowRounding)
  float PopupBorderSize;  // Thickness of border around popup/tooltip windows.
                          // Generally set to 0.0f or 1.0f. (Other values are
                          // not well tested and more CPU/GPU costly).
  Math::Vector2 FramePadding;  // Padding within a framed rectangle (used by
                               // most
                               // widgets).
  float FrameRounding;  // Radius of frame corners rounding. Set to 0.0f to have
                        // rectangular frame (used by most widgets).
  float FrameBorderSize;  // Thickness of border around frames. Generally set to
                          // 0.0f or 1.0f. (Other values are not well tested and
                          // more CPU/GPU costly).
  Math::Vector2
      ItemSpacing;  // Horizontal and vertical spacing between widgets/lines.
  Math::Vector2 ItemInnerSpacing;  // Horizontal and vertical spacing between
                                   // within
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
  float
      GrabMinSize;  // Minimum width/height of a grab box for slider/scrollbar.
  float GrabRounding;  // Radius of grabs corners rounding. Set to 0.0f to have
                       // rectangular slider grabs.
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
  bool AntiAliasedLines;  // Enable anti-aliasing on lines/borders. Disable if
                          // you are really tight on CPU/GPU.
  bool AntiAliasedFill;   // Enable anti-aliasing on filled shapes (rounded
                          // rectangles, circles, etc.)
  float CurveTessellationTol;  // Tessellation tolerance when using
                               // PathBezierCurveTo() without a specific number
                               // of segments. Decrease for highly tessellated
                               // curves (higher quality, more polygons),
                               // increase to reduce quality.
  Math::Vector4
      ColorStyle[static_cast<int>(GUIColorStyles::COUNT)];  // TODO(JACOB)

  GUIStyle();                              // TODO(JACOB)
  void ScaleAllSizes(float scale_factor);  // TODO(JACOB)
};
}  // namespace Isetta
