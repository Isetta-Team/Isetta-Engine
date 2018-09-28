/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "SID/sid.h"

#include "Core/IsettaAlias.h"
#include "Graphics/GUIStyle.h"

namespace Isetta {
class Color;
}

namespace Isetta::Math {
class Rect;
class Vector2;
class Vector2Int;
class Vector3;
class Vector4;
}  // namespace Isetta::Math

typedef void* TextureID;
class ImFont;
using Font = ImFont;
class ImGuiInputTextCallbackData;
using GUIInputTextCallbackData = ImGuiInputTextCallbackData;
using GUIInputTextCallback = int (*)(GUIInputTextCallbackData*);

namespace Isetta {
class GUI {
 public:
  // TODO(Jacob) Remove from public
  static void OnUpdate(const Action<>&);

  // BUTTONS
  static bool Button(const Math::Rect& position, const std::string& label,
                     const GUIStyle::ButtonStyle& style = {},
                     bool repeating = false);
  static void Button(const Math::Rect& position, const std::string& label,
                     const Action<>& callback,
                     const GUIStyle::ButtonStyle& style = {},
                     bool repeating = false);
  static bool ButtonImage(const Math::Rect& position, const std::string& id,
                          const TextureID& textureId,
                          const GUIStyle::ButtonStyle& style = {},
                          const GUIStyle::ImageStyle& imgStyle = {},
                          bool repeating = false, int framePadding = -1);
  static void ButtonImage(const Math::Rect& position, const std::string& id,
                          const TextureID& textureId, const Action<>& callback,
                          const GUIStyle::ButtonStyle& btnStyle = {},
                          const GUIStyle::ImageStyle& imgStyle = {},
                          bool repeating = false, int framePadding = -1);
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static bool ButtonArrow(const Math::Rect& position, const std::string& id,
                          GUIDir dir, const GUIStyle& style = NULL,
                          bool repeating = false);
  static void ButtonArrow(const Math::Rect& position, const std::string& id,
                          GUIDir dir, const Action<>& callback,
                          const GUIStyle& style = NULL, bool repeating =
                          false);
  static bool ButtonInline(const Math::Rect& position, const std::string&
  label,
                           const GUIStyle& style = NULL,
                           bool repeating = false);
  static void ButtonInline(const Math::Rect& position, const std::string&
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
  // TODO(Jacob) Styling
  static bool Toggle(
      const Math::Rect& position, const std::string& label, bool* value,
      const GUIStyle::ButtonStyle& style = {},
      const Color& check = GUIStyle{}.Colors[(int)GUIColorStyles::CheckMark]);
  static void Toggle(
      const Math::Rect& position, const std::string& label, bool* value,
      const Action<>& callback, const GUIStyle::ButtonStyle& style = {},
      const Color& check = GUIStyle{}.Colors[(int)GUIColorStyles::CheckMark]);
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
  static void Text(const Math::Rect& position, const GUIStyle::TextStyle& style,
                   const std::string format, ...);
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void Bullet();
  */
  // TODO(Jacob) styling
  static void Label(const Math::Rect& position, const std::string& label,
                    const Color& color, const std::string format, ...);
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  // TODO(Jacob) Vertical text?
  static bool Selectable(const Math::Rect& position, const std::string&
  label,
                         bool* selected,
                         SelectableFlags flags = SelectableFlags::None,
                         const Math::Vector2& size = Math::Vector2(0, 0));
  // TODO(Jacob) does having a callback for this make sense?
  static void Selectable(const Math::Rect& position, const std::string&
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
  static void InputText(const Math::Rect& position, const std::string& label,
                        char* buffer, int bufferSize,
                        const GUIStyle::InputStyle& style = {},
                        GUIInputTextFlags flags = GUIInputTextFlags::None,
                        const GUIInputTextCallback& callback = NULL);
  static void InputInt(const Math::Rect& position, const std::string& label,
                       int* value, const GUIStyle::InputStyle& style = {},
                       int step = 1, int stepFast = 100,
                       GUIInputTextFlags flags = GUIInputTextFlags::None);
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void InputFloat(const Math::Rect& position, const std::string&
  label,
                         float* value, float step = 0.0f, float stepFast =
                         0.0f, const std::string& format = "%.3f",
                         InputTextFlags flags = InputTextFlags::None);
  static void InputVector2(const Math::Rect& position, const std::string&
  label,
                           Math::Vector2* value, float step = 0.0f,
                           float stepFast = 0.0f,
                           const std::string& format = "%.3f",
                           InputTextFlags flags = InputTextFlags::None);
  static void InputVector3(const Math::Rect& position, const std::string&
  label,
                           Math::Vector3* value, float step = 0.0f,
                           float stepFast = 0.0f,
                           const std::string& format = "%.3f",
                           InputTextFlags flags = InputTextFlags::None);
  static void InputVector4(const Math::Rect& position, const std::string& label,
                           Math::Vector4* value, float step = 0.0f,
                           float stepFast = 0.0f,
                           const std::string& format = "%.3f",
                           GUIInputTextFlags flags = GUIInputTextFlags::None);
  // TODO(Jacob) InputVector2/3/4Int
  */

  // SLIDER
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  // TODO(Jacob) refactor
  struct SliderStyle {
    bool isVertical;
    Color color;
  };
  static void SliderInt(const Math::Rect& position, const std::string& label,
                        int* value, int minValue, int maxValue,
                        const SliderStyle& style = NULL,
                        const std::string& format = "%d");
  static void SliderInt(const Math::Rect& position, const std::string& label,
                        int* value, const Math::Vector2Int& range,
                        const SliderStyle& style = NULL,
                        const std::string& format = "%d");
  static void SliderFloat(const Math::Rect& position, const std::string&
  label,
                          float* value, float minValue, float maxValue,
                          const SliderStyle& style = NULL,
                          const std::string& format = "%.3f");
  static void SliderFloat(const Math::Rect& position, const std::string&
  label,
                          float* value, const Math::Vector2& range,
                          const SliderStyle& style = NULL,
                          const std::string& format = "%.3f");
  // TODO(Jacob) SliderVector2/3/4 Float/Int
  */

  // DRAG
  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void DragInt(const Math::Rect& position, const std::string& label,
                      int* value, const int min, const int max, float speed =
                      1, const std::string& format = "%d");
  static void DragInt(const Math::Rect& position, const std::string& label,
                      int* value, const Math::Vector2Int& range, int speed =
                      1, const std::string& format = "%d");
  static void DragFloat(const Math::Rect& position, const std::string& label,
                        float* value, const float min, const float max,
                        float speed = 1, const std::string& format = "%.3f");
  static void DragFloat(const Math::Rect& position, const std::string& label,
                        float* value, const Math::Vector2& range,
                        float speed = 1, const std::string& format = "%.3f");
  // TODO(Jacob) DragVector2/3/4 Float/Int
  */

  ////////////////////////////////////////
  // TODO(Jacob) NOT PART OF GAME NEEDS //
  ////////////////////////////////////////
  /*
  static void ColorPicker(const Math::Rect& position, const std::string&
  label,
                          Color* color,
                          ColorEditFlags flags = ColorEditFlags::NoAlpha);
  static void ComboBox(const Math::Rect& position, const std::string& label,
                       int* current, const std::string* items[], int length);
  static void ListBox(const Math::Rect& position, const std::string& label,
                      int* value, const std::string* items[], int length);
  */

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
  static void HorizontalGroup(const Math::Rect& position, const Action<>&
  ui); static void Child(const Math::Rect& position, const std::string& id,
                    const Action<>& ui, bool border = false,
                    GUIWindowFlags flags = GUIWindowFlags::None);
  */

  // WINDOWS
  static bool Window(const Math::Rect& position, const std::string& name,
                     const Action<>& ui, bool* isOpen = NULL,
                     const GUIStyle::BackgroundStyle& style = {},
                     const GUIWindowFlags flags = GUIWindowFlags::None);
  static bool MenuBar(const Action<>& ui, bool main = false,
                      const GUIStyle::BackgroundStyle& style = {});
  static bool Menu(const std::string& label, const Action<>& ui,
                   bool enabled = true);
  static bool MenuItem(const std::string& label, const std::string& shortcut,
                       bool* selected = false, bool enabled = true);
  static void MenuItem(const std::string& label, const std::string& shortcut,
                       const Action<>& callback, bool enabled = true);
  static bool Modal(const std::string& name, const Action<>& ui,
                    bool* isOpen = NULL,
                    const GUIStyle::BackgroundStyle& style = {},
                    GUIWindowFlags flags = GUIWindowFlags::None);
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
  struct Draw {
    static void Rect(const Math::Rect& rect, const Color& color,
                     const float roundCorners = 0.0f,
                     GUIDrawCornerFlags flags = GUIDrawCornerFlags::All,
                     float thickness = 1.0f);
    static void RectFilled(const Math::Rect& rect, const Color& color,
                           const float roundCorners = 0.0f,
                           GUIDrawCornerFlags flags = GUIDrawCornerFlags::All);
    static void Quad(const Math::Vector2&, const Math::Vector2&,
                     const Math::Vector2&, const Math::Vector2&, Color color,
                     float thickness = 1.0f);
    static void QuadFilled(const Math::Vector2&, const Math::Vector2&,
                           const Math::Vector2&, const Math::Vector2&,
                           Color color);
    static void Triangle(const Math::Vector2&, const Math::Vector2&,
                         const Math::Vector2&, Color color,
                         float thickness = 1.0f);
    static void TriangleFilled(const Math::Vector2&, const Math::Vector2&,
                               const Math::Vector2&, Color color);
    static void Circle(const Math::Vector2& center, float radius, Color color,
                       int segments = 12, float thickness = 1.0f);
    static void CircleFilled(const Math::Vector2& center, float radius,
                             Color color, int segments = 12);
    // TODO(Jacob) Do we allow DrawLine? 3017
  };

  static void Image(const Math::Rect& position, const TextureID& textureId,
                    const Math::Vector2& size,
                    const GUIStyle::ImageStyle& style = {});

  static void ProgressBar(const Math::Rect& position, float fraction,
                          const std::string& overlay = "",
                          const GUIStyle::ProgressBarStyle& style = {});
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
  // TODO(Jacob) Font
  // static void PushFont(Font* font)
  // static void PopFont();
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
  // TODO(Jacob) Fonts
  // static Font* GetFont();

  // TODO(Jacob) have font map?
  static Font* AddFontFromFile(const std::string& filename, int fontSize);
  static Font* AddFontFromMemory(void* fontBuffer, int fontSize, float pixels);
  static GUIStyle GetStyle();

 private:
  static class GUIModule* guiModule;
  friend class GUIModule;

  static bool Button(Func<bool> button, const Math::Rect& position,
                     const GUIStyle::ButtonStyle& style, bool repeating);
};
}  // namespace Isetta
