/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/GUI.h"

#include "Core/Color.h"
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Util.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector2Int.h"
#include "Core/Math/Vector3.h"
#include "Graphics/Font.h"
#include "Graphics/GUIModule.h"
#include "Graphics/GUIStyle.h"
#include "Graphics/RectTransform.h"
#include "Graphics/Texture.h"
#include "Util.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Isetta {
GUIModule* GUI::guiModule{nullptr};

// BUTTON
bool GUI::Button(Func<bool> button, const RectTransform& transform,
                 const ButtonStyle& style, bool repeating) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));

  ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)style.hover);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)style.active);
  ImGui::PushAllowKeyboardFocus(repeating);
  bool pressed = button();
  ImGui::PopAllowKeyboardFocus();
  ImGui::PopStyleColor(3);
  return pressed;
}
bool GUI::Button(const RectTransform& transform, const std::string& label,
                 const ButtonStyle& style, bool repeating) {
  Func<bool> btn;
  btn = std::bind(ImGui::Button, label.c_str(), (ImVec2)transform.rect.Size());
  return Button(btn, transform, style, repeating);
}
bool GUI::Button(const RectTransform& transform, const std::string& label,
                 const Action<>& callback, const ButtonStyle& style,
                 bool repeating) {
  if (Button(transform, label, style, repeating)) {
    callback();
    return true;
  }
  return false;
}
bool GUI::ButtonImage(const RectTransform& transform, const std::string& id,
                      const Texture& texture, const ButtonStyle& btnStyle,
                      const ImageStyle& imgStyle, bool repeating) {
  ButtonStyle style =
      ButtonStyle{imgStyle.frame, btnStyle.hover, btnStyle.active};
  Func<bool> btn =
      std::bind(ImGui::ImageButton,
                reinterpret_cast<void*>((intptr_t)texture.GetTexture()),
                (ImVec2)transform.rect.Size(), (ImVec2)imgStyle.offset,
                (ImVec2)imgStyle.tiling, imgStyle.framePadding,
                (ImVec4)btnStyle.background, (ImVec4)imgStyle.tint);
  return Button(btn, transform, style, repeating);
}
bool GUI::ButtonImage(const RectTransform& transform, const std::string& id,
                      const Texture& texture, const Action<>& callback,
                      const ButtonStyle& btnStyle, const ImageStyle& imgStyle,
                      bool repeating, int framePadding) {
  if (ButtonImage(transform, id, texture, btnStyle, imgStyle, repeating)) {
    callback();
    return true;
  }
  return false;
}
////////////////////////////////////////
// TODO(Jacob) NOT PART OF GAME NEEDS //
////////////////////////////////////////
/*
bool GUI::ButtonArrow(const RectTransform& transform, const std::string& id,
                      GUIDir dir, const GUIStyle& style, bool repeating) {
  Func<bool> btn = std::bind(ImGui::ArrowButton, id.c_str(), (ImGuiDir)dir);
  ImGui::PushStyleVar((ImGuiStyleVar)GUIStyleVar::FramePadding,
                      Math::Utility::Min({position.width, position.height}));
  bool pressed = Button(position, (id + "##arrow").c_str(), style, repeating) ||
                 ButtonStyle(btn, position, style, repeating);
  ImGui::PopStyleVar();
  return pressed;
}
void GUI::ButtonArrow(const RectTransform& transform, const std::string& id,
                      GUIDir dir, const Action<>& callback,
                      const GUIStyle& style = NULL, bool repeating = false) {
  if (ButtonArrow(position, id, dir, style, repeating)) {
    callback();
  }
}
bool GUI::ButtonInline(const RectTransform& transform, const std::string& label,
                       const GUIStyle& style = NULL, bool repeating = false) {
  Func<bool> btn = std::bind(ImGui::SmallButton, label.c_str());
  return ButtonStyle(btn, position, style, repeating);
}
void GUI::ButtonInline(const RectTransform& transform, const std::string& label,
                       const Action<>& callback, const GUIStyle& style = NULL,
                       bool repeating = false) {
  if (ButtonInline(position, label, style, repeating)) {
    callback();
  }
}
*/
void GUI::Toggle(const RectTransform& transform, const std::string& label,
                 bool* value, const ButtonStyle& style, const Color& check) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)style.hover);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)style.active);
  ImGui::PushStyleColor(ImGuiCol_CheckMark, (ImVec4)check);
  bool pressed = ImGui::Checkbox(label.c_str(), value);
  ImGui::PopItemWidth();
  ImGui::PopStyleColor(4);
}
void GUI::Toggle(const RectTransform& transform, const std::string& label,
                 bool* value, const Action<>& callback,
                 const ButtonStyle& style, const Color& check) {
  bool lastFrame = value;
  Toggle(transform, label, value, style, check);
  if (*value != lastFrame) {
    callback();
  }
}

// TEXT
void GUI::Text(const RectTransform& transform, const std::string_view format,
               const TextStyle& style) {
  Font::PushFont(style.font);
  const char* formatCStr = format.data();
  RectTransform rectTransform{transform};
  if (rectTransform.rect.width <= 0)
    rectTransform.rect.width = ImGui::CalcTextSize(formatCStr).x;
  if (rectTransform.rect.height <= 0)
    rectTransform.rect.height = ImGui::CalcTextSize(formatCStr).y;
  ImGui::SetCursorPos((ImVec2)SetPosition(rectTransform));
  ImGui::PushItemWidth(rectTransform.rect.width);

  ImGuiContext* context = ImGui::GetCurrentContext();
  ImGui::PushStyleColor(ImGuiCol_Text,
                        !style.isDisabled
                            ? (ImVec4)style.text
                            : context->Style.Colors[ImGuiCol_TextDisabled]);
  bool need_wrap = style.isWrapped &&
                   (context->CurrentWindow->DC.TextWrapPos <
                    0.0f);  // Keep existing wrap position is one ia already set
  ImGui::Text(formatCStr);
  if (need_wrap) {
    ImGui::PushTextWrapPos(0.0f);
  }
  ImGui::PopStyleColor();
  ImGui::PopItemWidth();
  Font::PopFont();
}

// Doesn't seem like its needed
// void GUI::Label(const RectTransform& transform, const std::string_view&
// label,
//                const std::string_view& format, const LabelStyle& style) {
//  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
//  ImGui::PushItemWidth(transform.rect.width);
//
//  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.text);
//  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
//  ImGui::PushID(label.data());
//  ImGui::Text(label.data());
//  ImGui::SameLine();
//  ImGui::LabelText("##label", format.data());
//  ImGui::PopID();
//  ImGui::PopStyleColor(2);
//  ImGui::PopItemWidth();
//}

// INPUT
bool GUI::InputText(const RectTransform& transform,
                    const std::string_view& label, char* buffer, int bufferSize,
                    const InputStyle& style, InputTextFlags flags,
                    InputTextCallback callback, void* userData) {
  // ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  // ImGui::PushItemWidth(transform.rect.width);
  RectTransform rect{transform};
  const char* labelCStr = label.data();
  ImVec2 textSize = ImGui::CalcTextSize(labelCStr);
  rect.rect.height = textSize.y;
  ImGui::SetCursorPos((ImVec2)SetPosition(rect));

  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)style.hovered);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)style.active);
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.text);
  ImGui::PushID(labelCStr);
  ImGui::Text(labelCStr);
  ImGui::SameLine();

  rect.rect.x += textSize.x + GUI::GetStyle().ItemSpacing.x;
  rect.rect.y -= GUI::GetStyle().FramePadding.y;
  ImGui::SetCursorPos((ImVec2)SetPosition(rect));
  rect.rect.width -= textSize.x + GUI::GetStyle().ItemSpacing.x;
  ImGui::PushItemWidth(rect.rect.width);
  bool input = ImGui::InputText("##input_text", buffer, bufferSize,
                                ImGuiInputTextFlags(flags), callback, userData);
  ImGui::PopID();
  ImGui::PopItemWidth();
  ImGui::PopStyleColor(4);
  return input;
}
void GUI::InputInt(const RectTransform& transform,
                   const std::string_view& label, int* value,
                   const InputStyle& style, int step, int stepFast,
                   InputTextFlags flags) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)style.hovered);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)style.active);
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.text);
  ImGui::PushID(label.data());
  ImGui::Text(label.data());
  ImGui::SameLine();
  ImGui::InputInt("##input_int", value, step, stepFast,
                  ImGuiInputTextFlags(flags));
  ImGui::PopID();
  ImGui::PopItemWidth();
  ImGui::PopStyleColor(4);
}
void GUI::InputVector3(const RectTransform& transform,
                       const std::string_view& label, Math::Vector3* value,
                       float step, const InputStyle& style,
                       const std::string_view& format, InputTextFlags flags) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)style.hovered);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)style.active);
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.text);
  ImGui::PushID(label.data());
  ImGui::Text(label.data());
  ImGui::SameLine();
  ImGui::InputFloat3("##input_vector3", value->xyz, format.data(),
                     (ImGuiInputTextFlags)flags);
  ImGui::PopID();
  ImGui::PopItemWidth();
  ImGui::PopStyleColor(4);
}

void GUI::SliderFloat(const RectTransform& transform,
                      const std::string_view& label, float* value, float min,
                      float max, float power, const char* format,
                      const InputStyle& style) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)style.hovered);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)style.active);
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.text);
  ImGui::PushID(label.data());
  ImGui::Text(label.data());
  ImGui::SameLine();
  ImGui::SliderFloat("##slider_float", value, min, max, format, power);
  ImGui::PopID();
  ImGui::PopItemWidth();
  ImGui::PopStyleColor(4);
}

void GUI::ComboBox(const RectTransform& transform,
                   const std::string_view& label, int* current,
                   const std::string_view* items[], const int length,
                   const ComboStyle& style) {
  // Style
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);
  struct FuncHolder {
    static bool ItemGetter(void* data, int idx, const char** outStr) {
      *outStr = ((const std::string_view*)data)[idx].data();
      return true;
    }
  };
  ImGui::Combo(label.data(), current, &FuncHolder::ItemGetter, items, length,
               style.maxHeight);
}

void GUI::ComboBox(const RectTransform& transform,
                   const std::string_view& label, int* current,
                   const Array<std::string>& items, const ComboStyle& style) {
  // Style
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  int& idx = *current;
  if (ImGui::BeginCombo(label.data(), items[idx].c_str())) {
    for (int i = 0; i < items.Size(); ++i) {
      ImGui::Selectable(items[i].c_str(), idx == i);
    }
    ImGui::EndCombo();
  }
}

bool GUI::ButtonDropDown(const RectTransform& transform,
                         const std::string_view& label,
                         const Math::Vector2& btnSize, const Action<>& ui) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));

  ImGuiWindow* window = ImGui::GetCurrentWindow();

  float x = ImGui::GetCursorPosX();
  float y = ImGui::GetCursorPosY();
  Math::Vector2 size{btnSize.x, btnSize.y};
  bool pressed = ImGui::Button("##", (ImVec2)size);

  // Arrow
  Math::Vector2 center(window->Pos.x + x + 0.5f * btnSize.x,
                       window->Pos.y + y + 0.5f * btnSize.y);
  float r = 8.f;
  center.y -= r * 0.25f;
  Math::Vector2 a = center + Math::Vector2{0, r};
  Math::Vector2 b = center + Math::Vector2{-0.866f, -0.5f} * r;
  Math::Vector2 c = center + Math::Vector2{0.866f, -0.5f} * r;
  window->DrawList->AddTriangleFilled((ImVec2)a, (ImVec2)b, (ImVec2)c,
                                      ImGui::GetColorU32(ImGuiCol_Text));

  // Popup
  Math::Vector2 popupPos{window->Pos.x + x, window->Pos.y + y + btnSize.y};
  ImGui::SetNextWindowPos((ImVec2)popupPos);
  const char* labelC = label.data();
  if (pressed) ImGui::OpenPopup(labelC);

  if (ImGui::BeginPopup(labelC)) {
    ui();
    ImGui::EndPopup();
    return true;
  }
  return false;
}

// WINDOWS
bool GUI::Window(const RectTransform& transform, const std::string& name,
                 const Action<>& ui, bool* isOpen, const WindowStyle& style,
                 const WindowFlags flags) {
  if (isOpen && !*isOpen) return false;
  ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)style.background);
  ImGui::SetNextWindowBgAlpha(style.background.a);
  ImGui::SetNextWindowPos((ImVec2)SetPosition(transform), ImGuiCond_Once);
  if (transform.rect.Size() != Math::Vector2::zero) {
    ImGui::SetNextWindowSize((ImVec2)transform.rect.Size(), ImGuiCond_Once);
  }
  if (style.constraints.Position() != style.constraints.Size()) {
    ImGui::SetNextWindowSizeConstraints((ImVec2)style.constraints.Min(),
                                        (ImVec2)style.constraints.Max());
  }
  bool collapsed = ImGui::Begin(name.c_str(), isOpen, (ImGuiWindowFlags)flags);
  if (collapsed) {
    ui();
  }
  ImGui::End();
  ImGui::PopStyleColor();
  return collapsed;
}
void GUI::Child(const RectTransform& transform, const std::string& id,
                const Action<>& ui, bool border, WindowFlags flags) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::BeginChild(id.c_str(), (ImVec2)transform.rect.Size(), border,
                    (ImGuiWindowFlags)flags);
  ui();
  ImGui::EndChild();
}
bool GUI::MenuBar(const Action<>& ui, bool main, const BackgroundStyle& style) {
  if (style.enabled) {
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, (ImVec4)style.background);
  }
  bool opened = false;
  if (main && ImGui::BeginMainMenuBar()) {
    ui();
    ImGui::EndMainMenuBar();
    opened = true;
  } else if (!main && ImGui::BeginMenuBar()) {
    ui();
    ImGui::EndMenuBar();
    opened = true;
  }
  if (style.enabled) {
    ImGui::PopStyleColor();
  }
  return opened;
}
bool GUI::Menu(const std::string& label, const Action<>& ui, bool enabled) {
  if (ImGui::BeginMenu(label.c_str(), enabled)) {
    ui();
    ImGui::EndMenu();
    return true;
  }
  return false;
}
bool GUI::MenuItem(const std::string& label, const std::string& shortcut,
                   bool* selected, bool enabled) {
  return ImGui::MenuItem(label.c_str(), shortcut.c_str(), selected, enabled);
}
void GUI::MenuItem(const std::string& label, const std::string& shortcut,
                   const Action<>& callback, bool enabled) {
  // TODO(Jacob) the false may cause the callback not to be called
  if (ImGui::MenuItem(label.c_str(), shortcut.c_str(), false, enabled)) {
    callback();
  }
}
bool GUI::Modal(const RectTransform& transform, const std::string& name,
                const Action<>& ui, bool* isOpen, const ModalStyle& style,
                WindowFlags flags) {
  const char* nameC = name.c_str();
  ImGui::SetNextWindowPos((ImVec2)SetPosition(transform));
  if (transform.rect.x != 0 || transform.rect.y != 0) {
    ImGui::SetNextWindowSize((ImVec2)transform.rect.Size());
    if ((flags & WindowFlags::NoResize) == WindowFlags::NoResize) {
      ImGui::SetNextWindowSizeConstraints((ImVec2)transform.rect.Size(),
                                          (ImVec2)transform.rect.Size());
    }
  }
  ImGui::PushStyleColor(ImGuiCol_PopupBg, (ImVec4)style.window);
  ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, (ImVec4)style.background);
  if (ImGui::BeginPopupModal(nameC, isOpen, ImGuiWindowFlags(flags))) {
    ui();
    ImGui::EndPopup();
  }
  ImGui::PopStyleColor(2);
  return isOpen;
}
void GUI::OpenPopup(const std::string& id) { ImGui::OpenPopup(id.c_str()); }
void GUI::CloseCurrentPopup() { ImGui::CloseCurrentPopup(); }

// DRAWING
void GUI::Draw::Rect(const RectTransform& transform, const Color& color,
                     const float roundCorners, DrawCornerFlags flags,
                     float thickness) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddRect(
      (ImVec2)position, (ImVec2)(position + transform.rect.Size()),
      ImGui::GetColorU32((ImVec4)color), roundCorners, ImDrawCornerFlags(flags),
      thickness);
}
void GUI::Draw::RectFilled(const RectTransform& transform, const Color& color,
                           const float roundCorners, DrawCornerFlags flags) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddRectFilled(
      (ImVec2)position, (ImVec2)(position + transform.rect.Size()),
      ImGui::GetColorU32(ImVec4(color)), roundCorners,
      ImDrawCornerFlags(flags));
}
void GUI::Draw::Quad(const RectTransform& transform, const Math::Vector2& a,
                     const Math::Vector2& b, const Math::Vector2& c,
                     const Math::Vector2& d, const Color& color,
                     float thickness) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddQuad(
      (ImVec2)(position + a), (ImVec2)(position + b), (ImVec2)(position + c),
      (ImVec2)(position + d), ImGui::GetColorU32(ImVec4(color)), thickness);
}
void GUI::Draw::QuadFilled(const RectTransform& transform,
                           const Math::Vector2& a, const Math::Vector2& b,
                           const Math::Vector2& c, const Math::Vector2& d,
                           const Color& color) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddQuadFilled(
      (ImVec2)(position + a), (ImVec2)(position + b), (ImVec2)(position + c),
      (ImVec2)(position + d), ImGui::GetColorU32((ImVec4)color));
}
void GUI::Draw::Triangle(const RectTransform& transform, const Math::Vector2& a,
                         const Math::Vector2& b, const Math::Vector2& c,
                         const Color& color, float thickness) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddTriangle(
      (ImVec2)(position + a), (ImVec2)(position + b), (ImVec2)(position + c),
      ImGui::GetColorU32(ImVec4(color)), thickness);
}
void GUI::Draw::TriangleFilled(const RectTransform& transform,
                               const Math::Vector2& a, const Math::Vector2& b,
                               const Math::Vector2& c, const Color& color) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddTriangleFilled(
      (ImVec2)(position + a), (ImVec2)(position + b), (ImVec2)(position + c),
      ImGui::GetColorU32(ImVec4(color)));
}
void GUI::Draw::Circle(const RectTransform& transform, float radius,
                       const Color& color, int segments, float thickness) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddCircle((ImVec2)position, radius,
                                        ImGui::GetColorU32((ImVec4)color),
                                        segments, thickness);
}
void GUI::Draw::CircleFilled(const RectTransform& transform, float radius,
                             const Color& color, int segments) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddCircleFilled(
      (ImVec2)position, radius, ImGui::GetColorU32((ImVec4)color), segments);
}
void GUI::Image(const RectTransform& transform, const Texture& texture,
                const ImageStyle& style) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  if (style.framePadding > 0) {
    Draw::RectFilled(
        RectTransform{Math::Rect{transform.rect.Position(),
                                 transform.rect.Size() +
                                     style.framePadding * Math::Vector2::one},
                      transform.anchor, transform.pivot},
        style.frame);
  }
  ImGui::Image(reinterpret_cast<void*>((intptr_t)(texture.GetTexture())),
               (ImVec2)transform.rect.Size(), (ImVec2)style.offset,
               (ImVec2)style.tiling, (ImVec4)style.tint, (ImVec4)style.frame);
}
void GUI::ProgressBar(const RectTransform& transform, float fraction,
                      const std::string& overlay,
                      const ProgressBarStyle& style) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.overlayText);
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)style.bar);
  if (style.hoverChange) {
    ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, (ImVec4)style.hover);
  }
  ImGui::ProgressBar(fraction, (ImVec2)transform.rect.Size(),
                     overlay.empty() ? NULL : overlay.c_str());
  ImGui::PopStyleColor(3);
  if (style.hoverChange) {
    ImGui::PopStyleColor();
  }
}

bool GUI::IsWindowFocused(FocusedFlags flags) {
  return ImGui::IsWindowFocused(static_cast<ImGuiFocusedFlags>(flags));
}

bool GUI::IsItemClicked(MouseButton code) {
  return ImGui::IsItemClicked(static_cast<int>(code));
}

void GUI::PushID(std::string_view id) { ImGui::PushID(id.data()); }
void GUI::PopID() { ImGui::PopID(); }

void GUI::PushStyleVar(StyleVar var, float val) {
  ImGui::PushStyleVar((ImGuiStyleVar)var, val);
}
void GUI::PushStyleVar(StyleVar var, const Math::Vector2& val) {
  ImGui::PushStyleVar((ImGuiStyleVar)var, (ImVec2)val);
}
void GUI::PopStyleVar(int pops) { ImGui::PopStyleVar(pops); }
void GUI::PushStyleColor(ColorStyles var, const Color& color) {
  ImGui::PushStyleColor((ImGuiCol)var, (ImVec4)color);
}
void GUI::PopStyleColor(int pop) { ImGui::PopStyleColor(pop); }
GUIStyle GUI::GetStyle() { return (GUIStyle)ImGui::GetStyle(); }

Math::Vector2 GUI::SetPosition(const RectTransform& transform) {
  Math::Vector2 pivot = PivotPosition(
      Math::Rect{Math::Vector2::zero, transform.rect.Size()}, transform.pivot);
  Math::Vector2 titleBar =
      ImGui::GetCurrentWindow()->TitleBarHeight() * Math::Vector2::up;
  Math::Vector2 anchor = PivotPosition(
      Math::Rect{transform.rect.Position() +
                     //(Math::Vector2)ImGui::GetWindowPos() + titleBar,
                     titleBar,
                 // TODO(Jacob + Chaojie) Is there a get viewport
                 // function that I could call?
                 (Math::Vector2)ImGui::GetWindowSize() - titleBar},
      transform.anchor);
  return anchor - pivot;
}

Math::Vector2 GUI::PivotPosition(Math::Rect rect, GUI::Pivot point) {
  Math::Vector2 position = Math::Vector2{};
  int bitsSet = Util::CountSetBits(static_cast<int>(point));
  if ((point & Pivot::TopLeft) == Pivot::TopLeft) {
    position.x += rect.Position().x;
    position.y += rect.Position().y;
  }
  if ((point & Pivot::TopRight) == Pivot::TopRight) {
    position.x += rect.Size().x + rect.Position().x;
    position.y += rect.Position().y;
  }
  if ((point & Pivot::BotLeft) == Pivot::BotLeft) {
    position.x += rect.Position().x;
    position.y += rect.Size().y + rect.Position().y;
  }
  if ((point & Pivot::BotRight) == Pivot::BotRight) {
    position.x += rect.Size().x + rect.Position().x;
    position.y += rect.Size().y + rect.Position().y;
  }

  position.x /= bitsSet;
  position.y /= bitsSet;

  return position;
}

GUI::WindowFlags operator|(GUI::WindowFlags lhs, GUI::WindowFlags rhs) {
  return (GUI::WindowFlags)(
      static_cast<std::underlying_type<GUI::WindowFlags>::type>(lhs) |
      static_cast<std::underlying_type<GUI::WindowFlags>::type>(rhs));
}
GUI::WindowFlags operator&(GUI::WindowFlags lhs, GUI::WindowFlags rhs) {
  return (GUI::WindowFlags)(
      static_cast<std::underlying_type<GUI::WindowFlags>::type>(lhs) &
      static_cast<std::underlying_type<GUI::WindowFlags>::type>(rhs));
}
GUI::TreeNodeFlags operator|(GUI::TreeNodeFlags lhs, GUI::TreeNodeFlags rhs) {
  return (GUI::TreeNodeFlags)(
      static_cast<std::underlying_type<GUI::TreeNodeFlags>::type>(lhs) |
      static_cast<std::underlying_type<GUI::TreeNodeFlags>::type>(rhs));
}
GUI::InputTextFlags operator|(GUI::InputTextFlags lhs,
                              GUI::InputTextFlags rhs) {
  return (GUI::InputTextFlags)(
      static_cast<std::underlying_type<GUI::InputTextFlags>::type>(lhs) |
      static_cast<std::underlying_type<GUI::InputTextFlags>::type>(rhs));
}
GUI::ColorEditFlags operator|(GUI::ColorEditFlags lhs,
                              GUI::ColorEditFlags rhs) {
  return (GUI::ColorEditFlags)(
      static_cast<std::underlying_type<GUI::ColorEditFlags>::type>(lhs) |
      static_cast<std::underlying_type<GUI::ColorEditFlags>::type>(rhs));
}
GUI::DrawCornerFlags operator|(GUI::DrawCornerFlags lhs,
                               GUI::DrawCornerFlags rhs) {
  return (GUI::DrawCornerFlags)(
      static_cast<std::underlying_type<GUI::DrawCornerFlags>::type>(lhs) |
      static_cast<std::underlying_type<GUI::DrawCornerFlags>::type>(rhs));
}
GUI::SelectableFlags operator|(GUI::SelectableFlags lhs,
                               GUI::SelectableFlags rhs) {
  return (GUI::SelectableFlags)(
      static_cast<std::underlying_type<GUI::ColorEditFlags>::type>(lhs) |
      static_cast<std::underlying_type<GUI::ColorEditFlags>::type>(rhs));
}
GUI::HoveredFlags operator|(GUI::HoveredFlags lhs, GUI::HoveredFlags rhs) {
  return (GUI::HoveredFlags)(
      static_cast<std::underlying_type<GUI::HoveredFlags>::type>(lhs) |
      static_cast<std::underlying_type<GUI::HoveredFlags>::type>(rhs));
}
GUI::FocusedFlags operator|(GUI::FocusedFlags lhs, GUI::FocusedFlags rhs) {
  return (GUI::FocusedFlags)(
      static_cast<std::underlying_type<GUI::FocusedFlags>::type>(lhs) |
      static_cast<std::underlying_type<GUI::FocusedFlags>::type>(rhs));
}
GUI::Cond operator|(GUI::Cond lhs, GUI::Cond rhs) {
  return (GUI::Cond)(static_cast<std::underlying_type<GUI::Cond>::type>(lhs) |
                     static_cast<std::underlying_type<GUI::Cond>::type>(rhs));
}
GUI::Pivot operator|(GUI::Pivot lhs, GUI::Pivot rhs) {
  return (GUI::Pivot)(static_cast<std::underlying_type<GUI::Pivot>::type>(lhs) |
                      static_cast<std::underlying_type<GUI::Pivot>::type>(rhs));
}
GUI::Pivot operator&(GUI::Pivot lhs, GUI::Pivot rhs) {
  return (GUI::Pivot)(static_cast<std::underlying_type<GUI::Pivot>::type>(lhs) &
                      static_cast<std::underlying_type<GUI::Pivot>::type>(rhs));
}
}  // namespace Isetta
