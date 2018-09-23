/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/IsettaAlias.h"
#include "Graphics/GUIStyle.h"

namespace Isetta {
class Color;

namespace Math {
class Rect;
class Vector2;
class Vector3;
class Vector4;
}  // namespace Math
}  // namespace Isetta

namespace Isetta {
class GUI {
 public:
  // TODO(Jacob) Remove from public
  static void OnUpdate(const Action<>&);
  /**/
  // BUTTONS
  static bool Button(const Math::Rect& position, const std::string label,
                     bool repeating = false);
  static void Button(const Math::Rect& position, const std::string label,
                     const Action<>& callback, bool repeating = false);
  static bool ArrowButton(const Math::Rect& position, const std::string label,
                          bool repeating = false);
  static void ArrowButton(const Math::Rect& position, const std::string label,
                          const Action<>& callback, bool repeating = false);
  // TODO(Jacob) SmallButton?
  static bool Toggle(const Math::Rect& position, const std::string& label,
                     bool* value);
  static void Toggle(const Math::Rect& position, const std::string& label,
                     bool* value, Action<> callback);
  // TODO(Jacob) Toggle which actually changes a flagged bit?
  // TODO(Jacob) Toggle or Checkbox?
  static void RadioButton(const std::string& label, int* value,
                          int* buttonValue);

  // TEXT
  static void Text(const std::string& format, ...);
  static void TextWrapped(const std::string& format, ...);
  static void TextColored(
      const Color color, const std::string& format,
      ...);  // TODO(Jacob) do we want to collapse this into Text?
  static void TextDisabled(const std::string& format, ...);
  static void Bullet();
  static void BulletText(const char* format, ...);
  static void BulletText(const std::string format, ...);
  static void LabelText(const std::string& label, const std::string format,
                        ...);

  // INPUT
  // TODO(Jacob) do we need a callback for each of these?
  static void InputText(
      const std::string& label, std::string* buffer,
      InputTextFlags flags = InputTextFlags::None, Action<> callback = NULL,
      void* userData = NULL);  // TODO(Jacob) don't know what userData is
  static void InputInt(const std::string& label, int* value, int step = 1,
                       int stepFast = 100,
                       InputTextFlags flags = InputTextFlags::None);
  static void InputFloat(const std::string& label, float* value,
                         float step = 0.0f, float stepFast = 0.0f,
                         const std::string& format = "%.3f",
                         InputTextFlags flags = InputTextFlags::None);
  static void InputVector2(const std::string& label, Math::Vector2* value,
                           float step = 0.0f, float stepFast = 0.0f,
                           const std::string& format = "%.3f",
                           InputTextFlags flags = InputTextFlags::None);
  static void InputVector3(const std::string& label, Math::Vector2* value,
                           float step = 0.0f, float stepFast = 0.0f,
                           const std::string& format = "%.3f",
                           InputTextFlags flags = InputTextFlags::None);
  static void InputVector4(const std::string& label, Math::Vector2* value,
                           float step = 0.0f, float stepFast = 0.0f,
                           const std::string& format = "%.3f",
                           InputTextFlags flags = InputTextFlags::None);
  static void SliderInt(const std::string& label, int* value, int minValue,
                        int maxValue, const std::string& format);
  static void SliderFloat(const std::string& label, float* value,
                          float minValue, float maxValue,
                          const std::string& format);

  static void InputColor(const std::string& label, Color* color,
                         ColorEditFlags flags = ColorEditFlags::NoAlpha);
  static void ComboBoxItem(const std::string& label, int* current,
                           const std::string* items[], int length);
  static void ListBox(const std::string& label, int* value,
                      const std::string* items[], int length);

  // SPACING
  static void Indent(float indent = 0.0f);
  static void Unindent(float indent = 0.0f);
  static void Spacing();
  static void SameLine();  // TODO(Jacob) parameters
  static void BeginGroup();
  static void EndGroup();
  static void Separator();

  // WINDOWS
  static bool BeginWindow(const Math::Rect& position, const std::string& name,
                          bool* isOpen,
                          const GUIWindowFlags flags = GUIWindowFlags::None);
  static void EndWindow();

  static bool BeginMenuBar();
  static void EndMenuBar();

  static bool BeginMenu(const std::string& label, bool enabled);
  static void EndMenu();
  static bool MenuItem(const std::string& label, const std::string& shortcut,
                       bool* selected, bool enabled);
  static bool MenuItem(const std::string& label, const std::string& shortcut,
                       const Action<>& callback, bool enabled);
  static bool CollapsingHeader(const std::string& label, TreeNodeFlags flags);
  // TODO(Jacob) TreeNode
  // TODO(Jacob) TreePop

  // DRAWING
  static void PlotLines(const std::string& label, const float* values[],
                        int length, int offset, const std::string& overlay,
                        float minScale = FLT_MAX, float maxScale = FLT_MAX,
                        Math::Vector2 graphSize = Math::Vector2(0, 0),
                        int stride = sizeof(float));
  static void Quad(const Math::Rect& rect, const Color& color,
                   const float roundCorners = 0.0f,
                   DrawCornerFlags flags = DrawCornerFlags::All,
                   float thickness = 1.0f);
  static void QuadFilled(const Math::Rect& rect, const Color& color,
                         const float roundCorners = 0.0f,
                         DrawCornerFlags flags = DrawCornerFlags::All);

  static bool IsItemHovered();
  static bool IsItemClicked();

  // STYLING
  static void PushStyleID(int id);  // TODO(Jacob) GUI styles
  static void PopStyleID(int id);
  static void PushStyleParam(GUIStyleParameter style, float* value);
  static void PopStyleParam();
  static void GetStyle() const;  // TODO(Jacob) Style
                                 /**/
 private:
  static class GUIModule* guiModule;
  friend class GUIModule;
};
}  // namespace Isetta
