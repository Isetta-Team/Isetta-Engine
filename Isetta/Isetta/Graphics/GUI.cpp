/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/GUI.h"

#include "Core/Color.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Util.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector2Int.h"
#include "Core/Math/Vector3.h"
#include "Graphics/GUIModule.h"
#include "Graphics/GUIStyle.h"
#include "Graphics/RectTransform.h"
#include "Util.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Isetta {
GUIModule* GUI::guiModule{nullptr};

void GUI::OnUpdate(const Action<>& callback) { guiModule->OnUpdate(callback); }

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
void GUI::Button(const RectTransform& transform, const std::string& label,
                 const Action<>& callback, const ButtonStyle& style,
                 bool repeating) {
  bool pressed = Button(transform, label, style, repeating);
  if (pressed) {
    callback();
  }
}
bool GUI::ButtonImage(const RectTransform& transform, const std::string& id,
                      const TextureID& textureId, const ButtonStyle& btnStyle,
                      const ImageStyle& imgStyle, bool repeating,
                      int framePadding) {
  ButtonStyle style =
      ButtonStyle{imgStyle.frame, btnStyle.hover, btnStyle.active};
  Func<bool> btn =
      std::bind(ImGui::ImageButton, textureId, (ImVec2)transform.rect.Size(),
                (ImVec2)imgStyle.offset, (ImVec2)imgStyle.tiling, framePadding,
                (ImVec4)btnStyle.background, (ImVec4)imgStyle.tint);
  return Button(btn, transform, style, repeating);
}
void GUI::ButtonImage(const RectTransform& transform, const std::string& id,
                      const TextureID& textureId, const Action<>& callback,
                      const ButtonStyle& btnStyle, const ImageStyle& imgStyle,
                      bool repeating, int framePadding) {
  if (ButtonImage(transform, id, textureId, btnStyle, imgStyle, repeating,
                  framePadding)) {
    callback();
  }
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
bool GUI::Toggle(const RectTransform& transform, const std::string& label,
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
  return pressed;
}
void GUI::Toggle(const RectTransform& transform, const std::string& label,
                 bool* value, const Action<>& callback,
                 const ButtonStyle& style, const Color& check) {
  if (Toggle(transform, label, value, style, check)) {
    callback();
  }
}

// TEXT
void GUI::Text(const RectTransform& transform, const TextStyle& style,
               const std::string format, ...) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  ImGuiContext* context = ImGui::GetCurrentContext();
  ImGui::PushStyleColor(ImGuiCol_Text,
                        !style.isDisabled
                            ? ImVec4(style.text)
                            : context->Style.Colors[ImGuiCol_TextDisabled]);
  bool need_wrap = style.isWrapped &&
                   (context->CurrentWindow->DC.TextWrapPos <
                    0.0f);  // Keep existing wrap position is one ia already set
  va_list args;
  va_start(args, format);
  ImGui::TextV(format.c_str(), args);
  va_end(args);
  if (need_wrap) {
    ImGui::PushTextWrapPos(0.0f);
  }
  ImGui::PopStyleColor();
  ImGui::PopItemWidth();
}
void GUI::Label(const RectTransform& transform, const std::string& label,
                const LabelStyle& style, const std::string format, ...) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.text);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  va_list args;
  va_start(args, format);
  ImGui::LabelTextV(label.c_str(), format.c_str(), args);
  va_end(args);
  ImGui::PopStyleColor(2);
  ImGui::PopItemWidth();
}

// INPUT
void GUI::InputText(const RectTransform& transform, const std::string& label,
                    char* buffer, int bufferSize, const InputStyle& style,
                    InputTextFlags flags,
                    const GUIInputTextCallback& callback) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)style.hovered);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)style.active);
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.text);
  ImGui::InputText(label.c_str(), buffer, bufferSize,
                   ImGuiInputTextFlags(flags), callback, NULL);
  ImGui::PopItemWidth();
  ImGui::PopStyleColor(4);
}
void GUI::InputInt(const RectTransform& transform, const std::string& label,
                   int* value, const InputStyle& style, int step, int stepFast,
                   InputTextFlags flags) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::PushItemWidth(transform.rect.width);

  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.background);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)style.hovered);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)style.active);
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)style.text);
  ImGui::InputInt(label.c_str(), value, step, stepFast,
                  ImGuiInputTextFlags(flags));
  ImGui::PopItemWidth();
  ImGui::PopStyleColor(4);
}

// WINDOWS
bool GUI::Window(const RectTransform& transform, const std::string& name,
                 const Action<>& ui, bool* isOpen, const BackgroundStyle& style,
                 const WindowFlags flags) {
  if (style.enabled) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)style.background);
    ImGui::SetNextWindowBgAlpha(style.background.a);
  }
  ImGui::SetNextWindowPos((ImVec2)SetPosition(transform));
  ImGui::SetNextWindowSize((ImVec2)transform.rect.Size());
  if ((flags & WindowFlags::NoResize) == WindowFlags::NoResize) {
    ImGui::SetNextWindowSizeConstraints((ImVec2)transform.rect.Size(),
                                        (ImVec2)transform.rect.Size());
  }
  bool collapsed = ImGui::Begin(name.c_str(), isOpen, ImGuiWindowFlags(flags));
  if (collapsed) {
    ui();
  }
  ImGui::End();
  if (style.enabled) {
    ImGui::PopStyleColor();
  }
  return collapsed;
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
bool GUI::Modal(const std::string& name, const Action<>& ui, bool* isOpen,
                const BackgroundStyle& style, WindowFlags flags) {
  const char* nameC = name.c_str();
  bool opened = false;
  if (style.enabled) {
    ImGui::PushStyleColor(ImGuiCol_PopupBg, (ImVec4)style.background);
  }
  if (ImGui::BeginPopupModal(nameC, isOpen, ImGuiWindowFlags(flags))) {
    ui();
    ImGui::EndPopup();
    opened = true;
  }
  if (style.enabled) {
    ImGui::PopStyleColor();
  }
  return opened;
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
                     const Math::Vector2& d, Color color, float thickness) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddQuad(
      (ImVec2)(position + a), (ImVec2)(position + b), (ImVec2)(position + c),
      (ImVec2)(position + d), ImGui::GetColorU32(ImVec4(color)), thickness);
}
void GUI::Draw::QuadFilled(const RectTransform& transform,
                           const Math::Vector2& a, const Math::Vector2& b,
                           const Math::Vector2& c, const Math::Vector2& d,
                           Color color) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddQuadFilled(
      (ImVec2)(position + a), (ImVec2)(position + b), (ImVec2)(position + c),
      (ImVec2)(position + d), ImGui::GetColorU32((ImVec4)color));
}
void GUI::Draw::Triangle(const RectTransform& transform, const Math::Vector2& a,
                         const Math::Vector2& b, const Math::Vector2& c,
                         Color color, float thickness) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddTriangle(
      (ImVec2)(position + a), (ImVec2)(position + b), (ImVec2)(position + c),
      ImGui::GetColorU32(ImVec4(color)), thickness);
}
void GUI::Draw::TriangleFilled(const RectTransform& transform,
                               const Math::Vector2& a, const Math::Vector2& b,
                               const Math::Vector2& c, Color color) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddTriangleFilled(
      (ImVec2)(position + a), (ImVec2)(position + b), (ImVec2)(position + c),
      ImGui::GetColorU32(ImVec4(color)));
}
void GUI::Draw::Circle(const RectTransform& transform, float radius,
                       Color color, int segments, float thickness) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddCircle((ImVec2)position, radius,
                                        ImGui::GetColorU32((ImVec4)color),
                                        segments, thickness);
}
void GUI::Draw::CircleFilled(const RectTransform& transform, float radius,
                             Color color, int segments) {
  Math::Vector2 position = SetPosition(transform);
  ImGui::GetWindowDrawList()->AddCircleFilled(
      (ImVec2)position, radius, ImGui::GetColorU32((ImVec4)color), segments);
}
void GUI::Image(const RectTransform& transform, const TextureID& textureId,
                const ImageStyle& style) {
  ImGui::SetCursorPos((ImVec2)SetPosition(transform));
  ImGui::Image(textureId, (ImVec2)transform.rect.Size(), (ImVec2)style.offset,
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

Font* GUI::AddFontFromFile(const std::string& filename, int fontSize) {
  return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), fontSize);
}
Font* GUI::AddFontFromMemory(void* fontBuffer, int fontSize, float pixels) {
  return ImGui::GetIO().Fonts->AddFontFromMemoryTTF(fontBuffer, fontSize,
                                                    pixels);
}

GUIStyle GUI::GetStyle() { return (GUIStyle)ImGui::GetStyle(); }

Math::Vector2 GUI::SetPosition(const RectTransform& transform) {
  Math::Vector2 pivot = PivotPosition(
      Math::Rect{Math::Vector2::zero, transform.rect.Size()}, transform.pivot);
  Math::Vector2 offset = {};
  if ((transform.anchor | Pivot::Top) == Pivot::Top)
    offset.y += ImGui::GetFrameHeight();
  Math::Vector2 anchor =
      PivotPosition(Math::Rect{transform.rect.Position() + offset,
                               // TODO(Jacob + Chaojie) Is there a get viewport
                               // function that I could call?
                               guiModule->GetWindowSize() - offset},
                    transform.anchor);
  return anchor - pivot;
}

Math::Vector2 GUI::PivotPosition(Math::Rect rect, GUI::Pivot point) {
  Math::Vector2 position = Math::Vector2{};
  int bitsSet = Util::CountSetBits((int)point);
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
