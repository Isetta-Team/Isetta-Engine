/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <bitset>
#include <string>
#include "Core/Color.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"

class ImGuiInputTextCallbackData;
class ImGuiTextFilter;

namespace Isetta {
class RectTransform;
class GUIStyle;
using TextFilter = ImGuiTextFilter;
using InputTextCallbackData = ImGuiInputTextCallbackData;
using InputTextCallback = int (*)(InputTextCallbackData*);
template <typename T>
class Array;
namespace Math {
class Rect;
class Vector3;
}  // namespace Math
}  // namespace Isetta

namespace Isetta {
class ISETTA_API GUI {
 public:
  enum class ISETTA_API WindowFlags : U32 {
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
    AlwaysAutoResize =
        1 << 6,                // Resize every window to its content every frame
    NoSavedSettings = 1 << 8,  // Never load/save settings in .ini file
    NoInputs = 1 << 9,         // Disable catching mouse or keyboard
                               // inputs, hovering test with pass through.
    MenuBar = 1 << 10,         // Has a menu-bar
    HorizontalScrollbar =
        1 << 11,  // Allow horizontal scrollbar to appear (off by default). You
                  // may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior
                  // to calling Begin() to specify width. Read code in
                  // imgui_demo in the "Horizontal Scrolling" section.
    NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning
                                   // from hidden to visible state
    NoBringToFrontOnFocus =
        1 << 13,  // Disable bringing window to front when taking focus (e.g.
                  // clicking on it or programatically giving it focus)
    AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even
                                        // if ContentSize.y < Size.y)
    AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar
                                          // (even if ContentSize.x < Size.x)
    AlwaysUseWindowPadding =
        1 << 16,  // Ensure child windows without border uses
                  // style.WindowPadding (ignored by default for non-bordered
                  // child windows, because more convenient)
    NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
    NoNavFocus =
        1 << 19,  // No focusing toward this window with gamepad/keyboard
                  // navigation (e.g. skipped by CTRL+TAB)
    NoNav = NoNavInputs | NoNavFocus,

    // [Internal]
    NavFlattened = 1 << 23,  // [BETA] Allow gamepad/keyboard navigation to
                             // cross over parent border to this child (only use
                             // on child that have no scrolling!)
    ChildWindow = 1 << 24,   // Don't use! For internal use by BeginChild()
    Tooltip = 1 << 25,       // Don't use! For internal use by BeginTooltip()
    Popup = 1 << 26,         // Don't use! For internal use by BeginPopup()
    Modal = 1 << 27,         // Don't use! For internal use by BeginPopupModal()
    ChildMenu = 1 << 28      // Don't use! For internal use by BeginMenu()
  };

  enum class ISETTA_API TreeNodeFlags : U16 {
    None = 0,
    Selected = 1 << 0,  // Draw as selected
    Framed = 1 << 1,    // Full colored frame (e.g. for CollapsingHeader)
    AllowItemOverlap =
        1 << 2,  // Hit testing to allow subsequent widgets to overlap this one
    NoTreePushOnOpen =
        1 << 3,  // Don't do a TreePush() when open (e.g. for CollapsingHeader)
                 // = no extra indent nor pushing on ID stack
    NoAutoOpenOnLog =
        1
        << 4,  // Don't automatically and temporarily open node when Logging is
               // active (by default logging will automatically open tree nodes)
    DefaultOpen = 1 << 5,        // Default node to be open
    OpenOnDoubleClick = 1 << 6,  // Need double-click to open node
    OpenOnArrow = 1 << 7,  // Only open when clicking on the arrow part. If
                           // OpenOnDoubleClick is also set, single-click
                           // arrow or double-click all box to open.
    Leaf =
        1
        << 8,  // No collapsing, no arrow (use as a convenience for leaf nodes).
    Bullet = 1 << 9,  // Display a bullet instead of arrow
    FramePadding =
        1 << 10,  // Use FramePadding (even for an unframed text node) to
                  // vertically align text baseline to regular widget height.
                  // Equivalent to calling AlignTextToFramePadding().
    // SpanAllAvailWidth  = 1 << 11,  // FIXME: TODO: Extend
    // hit box horizontally even if not framed NoScrollOnOpen =
    // 1 << 12,  // FIXME: TODO: Disable automatic scroll on TreePop() if node
    // got just open and contents is not visible
    NavLeftJumpsBackHere = 1
                           << 13,  // (WIP) Nav: left direction may move to this
                                   // TreeNode() from any of its child (items
                                   // submitted between TreeNode and TreePop)
    CollapsingHeader = Framed | NoTreePushOnOpen | NoAutoOpenOnLog
  };

  enum class ISETTA_API ColorStyles {
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

  enum class ISETTA_API InputTextFlags : U32 {
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
    Password = 1 << 15,  // Password mode, display all characters as '*'
    NoUndoRedo =
        1 << 16,  // Disable undo/redo. Note that input text owns the text data
                  // while active, if you want to provide your own undo/redo
                  // stack you need e.g. to call ClearActiveID().
    CharsScientific =
        1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
    CallbackResize =
        1
        << 18,  // Allow buffer capacity resize + notify when the string wants
                // to be resized (for string types which hold a cache of their
                // Size) (see misc/stl/imgui_stl.h for an example of using this)
    // [Internal]
    Multiline = 1 << 20  // For internal use by InputTextMultiline()
  };

  enum class ISETTA_API ColorEditFlags : U32 {
    None = 0,
    NoAlpha = 1 << 1,  //              // ColorEdit, ColorPicker, ColorButton:
                       //              ignore Alpha component (read 3 components
                       //              from the input pointer).
    NoPicker = 1 << 2,  //              // ColorEdit: disable picker when
                        //              clicking on colored square.
    NoOptions =
        1 << 3,  //              // ColorEdit: disable toggling options menu
                 //              when right-clicking on inputs/small preview.
    NoSmallPreview =
        1 << 4,  //              // ColorEdit, ColorPicker: disable colored
                 //              square preview next to the inputs. (e.g. to
                 //              show only the inputs)
    NoInputs =
        1 << 5,  //              // ColorEdit, ColorPicker: disable inputs
                 //              sliders/text widgets (e.g. to show only
                 //              the small preview colored square).
    NoTooltip =
        1 << 6,  //              // ColorEdit, ColorPicker, ColorButton: disable
                 //              tooltip when hovering the preview.
    NoLabel =
        1 << 7,  //              // ColorEdit, ColorPicker: disable display
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
    AlphaBar = 1 << 16,  //              // ColorEdit, ColorPicker: show
                         //              vertical alpha bar/gradient in picker.
    AlphaPreview =
        1 << 17,  //              // ColorEdit, ColorPicker, ColorButton:
                  //              display preview as a transparent color over a
                  //              checkerboard, instead of opaque.
    AlphaPreviewHalf =
        1 << 18,    //              // ColorEdit, ColorPicker, ColorButton:
                    //              display half opaque / half checkerboard,
                    //              instead of opaque.
    HDR = 1 << 19,  //              // (WIP) ColorEdit: Currently only disable
                    //              0.0f..1.0f limits in RGBA edition (note: you
                    //              probably want to use Float
                    //              flag as well).
    RGB = 1 << 20,  // [Inputs]     // ColorEdit: choose one among RGB/HSV/HEX.
                    // ColorPicker: choose any combination using RGB/HSV/HEX.
    HSV = 1 << 21,  // [Inputs]     // "
    HEX = 1 << 22,  // [Inputs]     // "
    Uint8 = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton:
                      // _display_ values formatted as 0..255.
    Float =
        1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton:
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

  enum class ISETTA_API StyleVar {
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

  enum class ISETTA_API DrawCornerFlags : U8 {
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

  enum class ISETTA_API SelectableFlags {
    None = 0,
    DontClosePopups = 1 << 0,   // Clicking this don't close parent popup window
    SpanAllColumns = 1 << 1,    // Selectable frame can span all columns (text
                                // will still fit in current column)
    AllowDoubleClick = 1 << 2,  // Generate press events on double clicks too
    Disabled = 1 << 3           // Cannot be selected, display greyed out text
  };

  enum class ISETTA_API HoveredFlags : U8 {
    None = 0,  // Return true if directly over the item/window, not obstructed
               // by another window, not obstructed by an active popup or modal
               // blocking inputs under them.
    ChildWindows = 1 << 0,  // IsWindowHovered() only: Return true if any
                            // children of the window is hovered
    RootWindow = 1 << 1,  // IsWindowHovered() only: Test from root window (top
                          // most parent of the current hierarchy)
    AnyWindow =
        1 << 2,  // IsWindowHovered() only: Return true if any window is hovered
    AllowWhenBlockedByPopup =
        1 << 3,  // Return true even if a popup window is normally blocking
                 // access to this item/window
    // AllowWhenBlockedByModal     = 1 << 4,   // Return true
    // even if a modal popup window is normally blocking access to this
    // item/window. FIXME-TODO: Unavailable yet.
    AllowWhenBlockedByActiveItem =
        1 << 5,  // Return true even if an active item is blocking access to
                 // this item/window. Useful for Drag and Drop patterns.
    AllowWhenOverlapped = 1 << 6,  // Return true even if the position is
                                   // overlapped by another window
    AllowWhenDisabled = 1 << 7,    // Return true even if the item is disabled
    RectOnly = AllowWhenBlockedByPopup | AllowWhenBlockedByActiveItem |
               AllowWhenOverlapped,
    RootAndChildWindows = RootWindow | ChildWindows
  };

  enum class ISETTA_API FocusedFlags : U8 {
    None = 0,
    ChildWindows = 1 << 0,  // IsWindowFocused(): Return true if any children of
                            // the window is focused
    RootWindow = 1 << 1,  // IsWindowFocused(): Test from root window (top most
                          // parent of the current hierarchy)
    AnyWindow =
        1 << 2,  // IsWindowFocused(): Return true if any window is focused
    RootAndChildWindows = RootWindow | ChildWindows
  };

  enum class ISETTA_API Cond : U8 {
    Always = 1 << 0,        // Set the variable
    Once = 1 << 1,          // Set the variable once per runtime session (only
                            // the first call with succeed)
    FirstUseEver = 1 << 2,  // Set the variable if the object/window has no
                            // persistently saved data (no entry in .ini file)
    Appearing = 1 << 3  // Set the variable if the object/window is appearing
                        // after being hidden/inactive (or the first time)
  };

  // TODO(Jacob)
  // enum class GUIDir {};

  // TODO(Jacob) "Same" as GUIDrawCorners but with center do we merge?
  enum class ISETTA_API Pivot : U8 {
    TopLeft = 1 << 0,             // 0x1
    TopRight = 1 << 1,            // 0x2
    BotLeft = 1 << 2,             // 0x4
    BotRight = 1 << 3,            // 0x8
    Top = TopLeft | TopRight,     // 0x3
    Bot = BotLeft | BotRight,     // 0xC
    Left = TopLeft | BotLeft,     // 0x5
    Right = TopRight | BotRight,  // 0xA
    Center = Top | Bot | Left | Right,
  };

  ISETTA_API friend WindowFlags operator|(WindowFlags lhs, WindowFlags rhs);
  ISETTA_API friend WindowFlags operator&(WindowFlags lhs, WindowFlags rhs);
  ISETTA_API friend TreeNodeFlags operator|(TreeNodeFlags lhs,
                                            TreeNodeFlags rhs);
  ISETTA_API friend InputTextFlags operator|(InputTextFlags lhs,
                                             InputTextFlags rhs);
  ISETTA_API friend ColorEditFlags operator|(ColorEditFlags lhs,
                                             ColorEditFlags rhs);
  ISETTA_API friend DrawCornerFlags operator|(DrawCornerFlags lhs,
                                              DrawCornerFlags rhs);
  ISETTA_API friend SelectableFlags operator|(SelectableFlags lhs,
                                              SelectableFlags rhs);
  ISETTA_API friend HoveredFlags operator|(HoveredFlags lhs, HoveredFlags rhs);
  ISETTA_API friend FocusedFlags operator|(FocusedFlags lhs, FocusedFlags rhs);
  ISETTA_API friend Cond operator|(Cond lhs, Cond rhs);
  ISETTA_API friend Pivot operator|(Pivot lhs, Pivot rhs);
  ISETTA_API friend Pivot operator&(Pivot lhs, Pivot rhs);

  struct ISETTA_API BackgroundStyle {
    bool enabled = false;
    Color background;
    BackgroundStyle() = default;
  };
  struct ISETTA_API ButtonStyle {
    Color background;
    Color hover;
    Color active;
    ButtonStyle();
    ButtonStyle(Color background, Color hover, Color active)
        : background{background}, hover{hover}, active{active} {}
  };
  struct ISETTA_API ComboStyle {
    int maxHeight = -1;
    // ComboStyle() {}
    ComboStyle(int maxHeight = -1) : maxHeight{maxHeight} {}
  };
  struct ISETTA_API ImageStyle {
    Color tint = Color::white;
    Color frame = Color::clear;
    Math::Vector2 offset = Math::Vector2::zero;
    Math::Vector2 tiling = Math::Vector2::one;
    int framePadding = -1;
    ImageStyle() = default;
    ImageStyle(const Color& tint, const Color& frame, int framePadding)
        : tint{tint}, frame{frame}, framePadding{framePadding} {}
    ImageStyle(const Math::Vector2& offset, const Math::Vector2& tiling)
        : offset{offset}, tiling{tiling} {}
    ImageStyle(const Color& tint, const Color& frame, int framePadding,
               const Math::Vector2& offset, const Math::Vector2& tiling)
        : tint{tint},
          frame{frame},
          framePadding{framePadding},
          offset{offset},
          tiling{tiling} {}
  };
  struct ISETTA_API InputStyle {
    Color background;
    Color hovered;
    Color active;
    Color text;
    class Font* font;
    InputStyle();
    InputStyle(class Font* const font);
    InputStyle(const Color& background, const Color& hovered,
               const Color& active, const Color& text);
    InputStyle(const Color& background, const Color& hovered,
               const Color& active, const Color& text, class Font* const font)
        : background{background},
          hovered{hovered},
          active{active},
          text{text},
          font{font} {}
  };
  struct ISETTA_API LabelStyle {
    Color text;
    Color background;
    class Font* font;
    LabelStyle();
    LabelStyle(class Font* const font);
    LabelStyle(const Color& text, const Color& background);
    LabelStyle(const Color& text, const Color& background,
               class Font* const font)
        : text{text}, background{background}, font{font} {}
  };
  struct ISETTA_API ProgressBarStyle {
    Color background;
    Color bar;
    Color overlayText;
    bool hoverChange;
    Color hover;
    ProgressBarStyle();
    ProgressBarStyle(const Color& background, const Color& bar,
                     const Color& overlayText);
    ProgressBarStyle(const Color& background, const Color& bar,
                     const Color& overlayText, bool hoverChange,
                     const Color& hover)
        : background{background},
          bar{bar},
          overlayText{overlayText},
          hoverChange{hoverChange},
          hover{hover} {}
  };
  struct ISETTA_API ModalStyle {
    Color window;
    Color background;
    ModalStyle();
    ModalStyle(const Color& window, const Color& background)
        : window{window}, background{background} {}
  };
  // TODO(Jacob) refactor
  struct ISETTA_API TextStyle {
    bool isWrapped = false;
    bool isDisabled = false;
    class Font* font = nullptr;
    // TODO(Jacob) Not worth implementing now
    // bool isBulleted;
    Color text;
    TextStyle();
    TextStyle(const Color& text)
        : text{text}, isWrapped{false}, isDisabled{false} {}
    TextStyle(float fontSize, const std::string_view& fontName = "");
    TextStyle(class Font* const font);
    TextStyle(const Color& text, float fontSize,
              const std::string_view& fontName = "");
    TextStyle(const Color& text, class Font* const font)
        : text{text}, font{font} {}
    TextStyle(bool wrapped, bool disabled, const Color& text);
  };
  struct ISETTA_API WindowStyle {
    Color background;
    Math::Rect constraints;
    WindowStyle();
    WindowStyle(const Color& background) : background{background} {
      constraints = Math::Rect{};
    }
    WindowStyle(const Math::Rect& constraints);
    WindowStyle(const Color& background, const Math::Rect& constraints)
        : background{background}, constraints{constraints} {}
  };

  // BUTTONS
  static bool Button(const RectTransform& transform, const std::string& label,
                     const ButtonStyle& style = {}, bool repeating = false);
  static bool Button(const RectTransform& transform, const std::string& label,
                     const Action<>& callback, const ButtonStyle& style = {},
                     bool repeating = false);
  static bool ButtonImage(const RectTransform& transform, const std::string& id,
                          const class Texture& texture,
                          const ButtonStyle& style = {},
                          const ImageStyle& imgStyle = {},
                          bool repeating = false);
  static bool ButtonImage(const RectTransform& transform, const std::string& id,
                          const class Texture& texture,
                          const Action<>& callback,
                          const ButtonStyle& btnStyle = {},
                          const ImageStyle& imgStyle = {},
                          bool repeating = false, int framePadding = -1);
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static bool ButtonArrow(const RectTransform& transform, const std::string& id,
                          GUIDir dir, const GUIStyle& style = NULL,
                          bool repeating = false);
  static void ButtonArrow(const RectTransform& transform, const std::string& id,
                          GUIDir dir, const Action<>& callback,
                          const GUIStyle& style = NULL, bool repeating =
                          false);
  static bool ButtonInline(const RectTransform& transform, const std::string&
  label,
                           const GUIStyle& style = NULL,
                           bool repeating = false);
  static void ButtonInline(const RectTransform& transform, const std::string&
  label,
                           const Action<>& callback,
                           const GUIStyle& style = NULL,
                           bool repeating = false);
  // TODO(Jacob) is this needed?
  static void ColorButton(const std::string& id, const Color& color,
                          ColorEditFlags flags = ColorEditFlags::None,
                          const Math::Vector2& size = Math::Vector2(
                              0, 0));  // TODO(Jacob) merge with other button
  */
  // TODO(Jacob) Toggle or Checkbox?
  static void Toggle(
      const RectTransform& transform, const std::string& label, bool* value,
      const ButtonStyle& style = {},
      const Color& check = Color::
          grey);  // (Color)GetStyle().Colors[(int)ColorStyles::CheckMark]
  static void Toggle(const RectTransform& transform, const std::string& label,
                     bool* value, const Action<>& callback,
                     const ButtonStyle& style = {},
                     const Color& check = Color::grey);
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  // TODO(Jacob) Toggle flagged bits?
  static void RadioButton(const std::string& label, int* value,
                          int* buttonValue);
  static void RadioButton(const std::string& label, int* value,
                          int* buttonValue, const Action<int>& callback);
  */

  // TEXT
  static void Text(const RectTransform& transform,
                   const std::string_view format, const TextStyle& style = {});
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void Bullet();
  */
  // TODO(Jacob) styling, doesn't seem needed
  // static void Label(const RectTransform& transform,
  //                  const std::string_view& label,
  //                  const std::string_view& format,
  //                  const LabelStyle& style = {});
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  // TODO(Jacob) Vertical text?
  static bool Selectable(const RectTransform& transform, const std::string&
  label,
                         bool* selected,
                         SelectableFlags flags = SelectableFlags::None,
                         const Math::Vector2& size = Math::Vector2(0, 0));
  // TODO(Jacob) does having a callback for this make sense?
  static void Selectable(const RectTransform& transform, const std::string&
  label,
                         bool* selected, const Action<std::string>& callback,
                         SelectableFlags flags = SelectableFlags::None,
                         const Math::Vector2& size = Math::Vector2(0, 0));
  */

  // INPUT
  // TODO(Jacob) No other input has callback
  // TODO(Jacob) InputTextCallbackData* in action
  // Action<std::string&> callback = NULL,
  // TODO(Jacob) don't know what userData is
  // void* userData = NULL
  // TODO(Jacob)
  // struct InputTextCallbackData : ImGuiInputTextCallbackData {
  //  unsigned short eventChar;
  //  // TODO(Jacob) ImGuiKey
  //  void* userData;
  //  char* buffer;
  //  int bufferLen;
  //  int bufferSize;
  //  bool bufferDirty;
  //  Math::Vector2Int selection;
  //  InputTextCallbackData();
  //};
  static bool InputText(const RectTransform& transform,
                        const std::string_view& label, char* buffer,
                        int bufferSize, const InputStyle& style = {},
                        InputTextFlags flags = InputTextFlags::None,
                        InputTextCallback callback = NULL,
                        void* userData = NULL);
  static void InputInt(const RectTransform& transform,
                       const std::string_view& label, int* value,
                       const InputStyle& style = {}, int step = 1,
                       int stepFast = 100,
                       InputTextFlags flags = InputTextFlags::None);
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void InputFloat(const RectTransform& transform, const std::string&
  label,
                         float* value, float step = 0.0f, float stepFast =
                         0.0f, const std::string& format = "%.3f",
                         InputTextFlags flags = InputTextFlags::None);
  static void InputVector2(const RectTransform& transform, const std::string&
  label,
                           Math::Vector2* value, float step = 0.0f,
                           float stepFast = 0.0f,
                           const std::string& format = "%.3f",
                           InputTextFlags flags = InputTextFlags::None);
  static void InputVector4(const RectTransform& transform, const std::string&
  label, Math::Vector4* value, float step = 0.0f, float stepFast = 0.0f, const
  std::string& format = "%.3f", GUIInputTextFlags flags =
  GUIInputTextFlags::None);
  // TODO(Jacob) InputVector2/3/4Int
  */
  static void InputVector3(const RectTransform& transform,
                           const std::string_view& label, Math::Vector3* value,
                           float step = 0.0f, const InputStyle& style = {},
                           const std::string_view& format = "%.3f",
                           InputTextFlags flags = InputTextFlags::None);

  // SLIDER
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  static void SliderFloat(const RectTransform& transform,
                          const std::string_view& label, float* value,
                          float min, float max, float power = 1,
                          const char* format = "%.3f",
                          const InputStyle& style = {});
  /*
  // TODO(Jacob) refactor
  struct SliderStyle {
    bool isVertical;
    Color color;
  };
  static void SliderInt(const RectTransform& transform, const std::string&
  label, int* value, int minValue, int maxValue, const SliderStyle& style =
  NULL, const std::string& format = "%d"); static void SliderInt(const
  RectTransform& transform, const std::string& label, int* value, const
  Math::Vector2Int& range, const SliderStyle& style = NULL, const std::string&
  format = "%d"); static void SliderFloat(const RectTransform& transform, const
  std::string& label, float* value, float minValue, float maxValue, const
  SliderStyle& style = NULL, const std::string& format = "%.3f"); static void
  SliderFloat(const RectTransform& transform, const std::string& label, float*
  value, const Math::Vector2& range, const SliderStyle& style = NULL, const
  std::string& format = "%.3f");
  // TODO(Jacob) SliderVector2/3/4 Float/Int
  */

  // DRAG
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void DragInt(const RectTransform& transform, const std::string& label,
                      int* value, const int min, const int max, float speed =
                      1, const std::string& format = "%d");
  static void DragInt(const RectTransform& transform, const std::string& label,
                      int* value, const Math::Vector2Int& range, int speed =
                      1, const std::string& format = "%d");
  static void DragFloat(const RectTransform& transform, const std::string&
  label, float* value, const float min, const float max, float speed = 1, const
  std::string& format = "%.3f"); static void DragFloat(const RectTransform&
  transform, const std::string& label, float* value, const Math::Vector2& range,
                        float speed = 1, const std::string& format = "%.3f");
  // TODO(Jacob) DragVector2/3/4 Float/Int
  */

  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void ColorPicker(const RectTransform& transform, const std::string&
  label,
                          Color* color,
                          ColorEditFlags flags = ColorEditFlags::NoAlpha);

  static void ListBox(const RectTransform& transform, const std::string& label,
                      int* value, const std::string* items[], int length);
  */
  // Single Select
  static void ComboBox(const RectTransform& transform,
                       const std::string_view& label, int* current,
                       const std::string_view* items[], const int length,
                       const ComboStyle& style = {});
  static void ComboBox(const RectTransform& transform,
                       const std::string_view& label, int* current,
                       const Array<std::string>& items,
                       const ComboStyle& style = {});
  static bool ButtonDropDown(const RectTransform& transform,
                             const std::string_view& label,
                             const Math::Vector2& btnSize, const Action<>& ui);

  // LAYOUT/SPACING
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void Indent(float indent = 0.0f);
  static void Unindent(float indent = 0.0f);
  static void Spacing();
  static void SameLine(float xPos = 0.0f, float spacing = -1.0f);
  static void NewLine();
  static void Dummy(const Math::Vector2& size);
  // TODO(Jacob) automatic / manual wrapping
  // TODO(Jacob) AlignTextToFramePadding line 2252
  // TODO(Jacob) Do we need this static void VerticalGroup();
  static void Separator();
  static void Columns(int count, const std::string& id = NULL,
                      bool border = true);
  static void NextColumn();
  static void HorizontalGroup(const RectTransform& transform, const Action<>&
  ui);
  */

  // WINDOWS
  static bool Window(const RectTransform& transform, const std::string& name,
                     const Action<>& ui, bool* isOpen = NULL,
                     const WindowStyle& style = {},
                     const WindowFlags flags = WindowFlags::None);
  static void Child(const RectTransform& transform, const std::string& id,
                    const Action<>& ui, bool border = false,
                    WindowFlags flags = WindowFlags::None);
  static bool MenuBar(const Action<>& ui, bool main = false,
                      const BackgroundStyle& style = {});
  static bool Menu(const std::string& label, const Action<>& ui,
                   bool enabled = true);
  static bool MenuItem(const std::string& label, const std::string& shortcut,
                       bool* selected = false, bool enabled = true);
  static void MenuItem(const std::string& label, const std::string& shortcut,
                       const Action<>& callback, bool enabled = true);
  static bool Modal(const RectTransform& transform, const std::string& name,
                    const Action<>& ui, bool* isOpen = NULL,
                    const ModalStyle& style = {},
                    WindowFlags flags = WindowFlags::None);
  static void OpenPopup(const std::string& id);
  static void CloseCurrentPopup();
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static bool CollapsingHeader(const std::string& label, TreeNodeFlags
  flags); static bool TreeNode(const std::string& label);
  // TODO(Jacob) better name
  static void TreePop();
  static void OpenPopup(const std::string& id);
  static bool BeginPopup(const std::string& id,
                         GUIWindowFlags flags = GUIWindowFlags::None);
  static void EndPopup();
  static bool BeginContextItem(
      const std::string& id,
      int mouseButton);  // TODO(Jacob) do we have a mouse enum
  */
  // TODO(Jacob) DragDropTarget?

  // DRAWING
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void PlotLines(const std::string& label, const float* values[],
                        int length, int offset, const std::string& overlay,
                        float minScale = FLT_MAX, float maxScale = FLT_MAX,
                        Math::Vector2 graphSize = Math::Vector2(0, 0),
                        int stride = sizeof(float));
  static void PlotHistogram(const std::string& label, const float* values[],
                            int length, int offset = 0,
                            const std::string& overlay = NULL,
                            float minScale = FLT_MAX, float maxScale =
                            FLT_MAX, Math::Vector2 graphSize =
                            Math::Vector2(0, 0), int stride = sizeof(float));
  */
  struct ISETTA_API Draw {
    static void Rect(const RectTransform& transform, const Color& color,
                     const float roundCorners = 0.0f,
                     DrawCornerFlags flags = DrawCornerFlags::All,
                     float thickness = 1.0f);
    static void RectFilled(const RectTransform& transform, const Color& color,
                           const float roundCorners = 0.0f,
                           DrawCornerFlags flags = DrawCornerFlags::All);
    static void Quad(const RectTransform& transform, const Math::Vector2&,
                     const Math::Vector2&, const Math::Vector2&,
                     const Math::Vector2&, const Color& color,
                     float thickness = 1.0f);
    static void QuadFilled(const RectTransform& transform, const Math::Vector2&,
                           const Math::Vector2&, const Math::Vector2&,
                           const Math::Vector2&, const Color& color);
    static void Triangle(const RectTransform& transform, const Math::Vector2&,
                         const Math::Vector2&, const Math::Vector2&,
                         const Color& color, float thickness = 1.0f);
    static void TriangleFilled(const RectTransform& transform,
                               const Math::Vector2&, const Math::Vector2&,
                               const Math::Vector2&, const Color& color);
    static void Circle(const RectTransform& transform, float radius,
                       const Color& color, int segments = 12,
                       float thickness = 1.0f);
    static void CircleFilled(const RectTransform& transform, float radius,
                             const Color& color, int segments = 12);
    // TODO(Jacob) Do we allow DrawLine? 3017
  };

  static void Image(const RectTransform& transform,
                    const class Texture& texture, const ImageStyle& style = {});

  static void ProgressBar(const RectTransform& transform, float fraction,
                          const std::string& overlay = "",
                          const ProgressBarStyle& style = {});
  // TODO(Jacob) filter search 2845

  // CHECKERS
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static bool IsItemActive();
  static bool IsItemFocused();
  static bool IsItemEdited();
  static bool IsItemDeactivated();
  static bool IsItemDeactivatedAfterEdit();
  static bool IsItemVisible();
  static bool IsItemHovered(HoveredFlags flags = HoveredFlags::None);
  static bool IsItemClicked();
  static bool IsWindowFocused(FocusedFlags flags = FocusedFlags::None);
  static bool IsWindowHovered(HoveredFlags flags = HoveredFlags::None);
  static bool IsMouseDoubleClicked(
      int button);  // TODO(Jacob) should this be in input?
  // TODO(Chaojie) There are a lot of input examples that we might want
  // imgui_demo.cpp (2861)
  static void SetItemDefaultFocus();
  // TODO(Jacob) GUISizeCallbackData 4193
   static void SetNextWindowSizeConstaints(const Math::Vector2& minSize,
                                          const Math::Vector2& maxSize,
                                          GUISizeCallback callback,
                                          void* userData); // TODO(Jacob) is
                                          this needed?
  static void SetKeyboardFocusHere(
      int offset);  // TODO(Jacob) do we allow offset? if so change name
  */
  // TODO(Jacob) LogToTTY/LogToFile/LogToClipboard

  // STYLING
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  // TODO(Jacob) refactor
  static void PushStyleID(int id);  // TODO(Jacob) GUI styles
  static void PopStyleID(int id);   // TODO(Jacob) Is this a thing?
  static void PopStyleID();
  static void PushStyleParam(GUIStyleVar style, float* value);
  static void PopStyleParam();
  static void PushItemWidth(float width);  // TODO(Jacob) integrate into
  other
                                           // things (lines 2011-2047)
  static void PushAllowKeyboardFocus(bool allowFocus);  // TODO(Jacob) Merge
  static void PopAllowKeyboardFocus();
  static Math::Vector2 GetWindowSize();
  static void SetMouseCursor(MouseCursor type);
  // TODO(Jacob) Classic/Dark/Light sytling?
  */
  // TODO(Jacob) Load/SaveIniSettings?
  static void PushID(std::string_view id);
  static void PopID();
  static void PushStyleVar(StyleVar var, float val);
  static void PushStyleVar(StyleVar var, const Math::Vector2& val);
  static void PopStyleVar(int pops = 1);
  static void PushStyleColor(ColorStyles var, const Color& color);
  static void PopStyleColor(int pops = 1);
  static GUIStyle GetStyle();

 private:
  static class GUIModule* guiModule;
  friend class GUIModule;

  static bool Button(Func<bool> button, const RectTransform& transform,
                     const ButtonStyle& style, bool repeating);
  static Math::Vector2 SetPosition(const RectTransform& transform);
  static Math::Vector2 PivotPosition(Math::Rect rect, Pivot point);
};
}  // namespace Isetta
