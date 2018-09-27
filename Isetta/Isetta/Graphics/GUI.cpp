/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/GUI.h"

#include "Core/Color.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Utility.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector2Int.h"
#include "Core/Math/Vector3.h"
#include "Graphics/GUIModule.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Isetta {
GUIModule* GUI::guiModule{nullptr};

void GUI::OnUpdate(const Action<>& callback) { guiModule->OnUpdate(callback); }

// BUTTON
bool GUI::Button(Func<bool> button, const Math::Rect& position,
                 const ButtonStyle& style, bool repeating) {
  ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)style.color);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)style.hover);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)style.active);
  ImGui::SetCursorPos((ImVec2)position.Position());
  ImGui::PushAllowKeyboardFocus(repeating);
  bool pressed = button();
  ImGui::PopAllowKeyboardFocus();
  ImGui::PopStyleColor(3);
  return pressed;
}
bool GUI::Button(const Math::Rect& position, const std::string& label,
                 const ButtonStyle& style, bool repeating) {
  Func<bool> btn;
  btn = std::bind(ImGui::Button, label.c_str(), (ImVec2)position.Size());
  return Button(btn, position, style, repeating);
}
void GUI::Button(const Math::Rect& position, const std::string& label,
                 const Action<>& callback, const ButtonStyle& style,
                 bool repeating) {
  bool pressed = Button(position, label, style, repeating);
  if (pressed) {
    callback();
  }
}
bool GUI::ButtonImage(const Math::Rect& position, const std::string& id,
                      const TextureID& textureId, const ButtonStyle& inStyle,
                      bool repeating, const Math::Vector2& offset,
                      const Math::Vector2& tiling, int framePadding,
                      const Color& frameColor, const Color& tint) {
  ButtonStyle style = ButtonStyle{frameColor, inStyle.hover, inStyle.active};
  Func<bool> btn = std::bind(
      ImGui::ImageButton, textureId, (ImVec2)position.Size(), (ImVec2)offset,
      (ImVec2)tiling, framePadding, (ImVec4)inStyle.color, (ImVec4)tint);
  return Button(btn, position, style, repeating);
}
void GUI::ButtonImage(const Math::Rect& position, const std::string& id,
                      const TextureID& textureId, const Action<>& callback,
                      const ButtonStyle& style, bool repeating,
                      const Math::Vector2& offset, const Math::Vector2& tiling,
                      int framePadding, const Color& frameColor,
                      const Color& tint) {
  if (ButtonImage(position, id, textureId, style, repeating, offset, tiling,
                  framePadding, frameColor, tint)) {
    callback();
  }
}
////////////////////////////////////////
// TODO(Jacob) NOT PART OF GAME NEEDS //
////////////////////////////////////////
/*
bool GUI::ButtonArrow(const Math::Rect& position, const std::string& id,
                      GUIDir dir, const GUIStyle& style, bool repeating) {
  Func<bool> btn = std::bind(ImGui::ArrowButton, id.c_str(), (ImGuiDir)dir);
  ImGui::PushStyleVar((ImGuiStyleVar)GUIStyleVar::FramePadding,
                      Math::Utility::Min({position.width, position.height}));
  bool pressed = Button(position, (id + "##arrow").c_str(), style, repeating) ||
                 ButtonStyle(btn, position, style, repeating);
  ImGui::PopStyleVar();
  return pressed;
}
void GUI::ButtonArrow(const Math::Rect& position, const std::string& id,
                      GUIDir dir, const Action<>& callback,
                      const GUIStyle& style = NULL, bool repeating = false) {
  if (ButtonArrow(position, id, dir, style, repeating)) {
    callback();
  }
}
bool GUI::ButtonInline(const Math::Rect& position, const std::string& label,
                       const GUIStyle& style = NULL, bool repeating = false) {
  Func<bool> btn = std::bind(ImGui::SmallButton, label.c_str());
  return ButtonStyle(btn, position, style, repeating);
}
void GUI::ButtonInline(const Math::Rect& position, const std::string& label,
                       const Action<>& callback, const GUIStyle& style = NULL,
                       bool repeating = false) {
  if (ButtonInline(position, label, style, repeating)) {
    callback();
  }
}
*/
bool GUI::Toggle(const Math::Rect& position, const std::string& label,
                 bool* value, const ButtonStyle& style) {
  ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)style.color);
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)style.hover);
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)style.active);
  ImGui::SetCursorPos((ImVec2)position.Position());
  bool pressed = ImGui::Checkbox(label.c_str(), value);
  ImGui::PopStyleColor(3);
  return pressed;
}
void GUI::Toggle(const Math::Rect& position, const std::string& label,
                 bool* value, const Action<>& callback,
                 const ButtonStyle& style) {
  if (Toggle(position, label, value, style)) {
    callback();
  }
}

// TEXT
void GUI::Text(const Math::Rect& position, const TextStyle& style,
               const std::string format, ...) {
  ImGui::SetCursorPos((ImVec2)position.Position());
  ImGuiContext* context = ImGui::GetCurrentContext();
  ImGui::PushStyleColor(ImGuiCol_Text,
                        !style.isDisabled
                            ? ImVec4(style.color)
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
}
void GUI::Label(const Math::Rect& position, const std::string& label,
                const Color& color, const std::string format, ...) {
  ImGui::SetCursorPos((ImVec2)position.Position());
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color));
  va_list args;
  va_start(args, format);
  ImGui::LabelTextV(label.c_str(), format.c_str(), args);
  va_end(args);
  ImGui::PopStyleColor();
}

// INPUT
static int test(GUIInputTextCallbackData* data) {
  LOG_INFO(Debug::Channel::GUI, "TEST");
  return 0;
}
void GUI::InputText(const Math::Rect& position, const std::string& label,
                    char* buffer, int bufferSize, GUIInputTextFlags flags,
                    const GUIInputTextCallback& callback) {
  // TODO(Jacob) Styling
  ImGui::SetCursorPos((ImVec2)position.Position());
  ImGui::InputText(label.c_str(), buffer, bufferSize,
                   ImGuiInputTextFlags(flags), callback, NULL);
}
void GUI::InputInt(const Math::Rect& position, const std::string& label,
                   int* value, int step, int stepFast,
                   GUIInputTextFlags flags) {
  // TODO(Jacob) Styling
  ImGui::SetCursorPos((ImVec2)position.Position());
  ImGui::InputInt(label.c_str(), value, step, stepFast,
                  ImGuiInputTextFlags(flags));
}

// WINDOWS
bool GUI::Window(const Math::Rect& position, const std::string& name,
                 bool* isOpen, const Action<>& ui, const GUIWindowFlags flags) {
  bool collapsed = ImGui::Begin(name.c_str(), isOpen, ImGuiWindowFlags(flags));
  if (collapsed) {
    ui();
  }
  ImGui::End();
  return collapsed;
}
bool GUI::MenuBar(const Action<>& ui, bool main) {
  bool opened = false;
  if (main) {
    opened = ImGui::BeginMenuBar();
    if (opened) {
      ui();
    }
    ImGui::EndMenuBar();
  } else {
    opened = ImGui::BeginMenuBar();
    if (opened) {
      ui();
    }
    ImGui::EndMainMenuBar();
  }
  return opened;
}
bool GUI::Menu(const std::string& label, const Action<>& ui, bool enabled) {
  bool opened = ImGui::BeginMenu(label.c_str(), enabled);
  if (opened) {
    ui();
  }
  ImGui::EndMenu();
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
                GUIWindowFlags flags) {
  bool opened =
      ImGui::BeginPopupModal(name.c_str(), isOpen, ImGuiWindowFlags(flags));
  if (opened) {
    ui();
  }
  ImGui::EndPopup();
  return opened;
}
void GUI::CloseCurrentPopup() { ImGui::CloseCurrentPopup(); }

// DRAWING
void GUI::Draw::Rect(const Math::Rect& rect, const Color& color,
                     const float roundCorners, GUIDrawCornerFlags flags,
                     float thickness) {
  ImGui::GetWindowDrawList()->AddRect(
      ImVec2(rect.Min()), ImVec2(rect.Max()), ImGui::GetColorU32(ImVec4(color)),
      roundCorners, ImDrawCornerFlags(flags), thickness);
}
void GUI::Draw::RectFilled(const Math::Rect& rect, const Color& color,
                           const float roundCorners, GUIDrawCornerFlags flags) {
  ImGui::GetWindowDrawList()->AddRectFilled(
      ImVec2(rect.Min()), ImVec2(rect.Max()), ImGui::GetColorU32(ImVec4(color)),
      roundCorners, ImDrawCornerFlags(flags));
}
void GUI::Draw::Quad(const Math::Vector2& a, const Math::Vector2& b,
                     const Math::Vector2& c, const Math::Vector2& d,
                     Color color, float thickness) {
  ImGui::GetWindowDrawList()->AddQuad(
      ImVec2(a), ImVec2(b), ImVec2(c), ImVec2(d),
      ImGui::GetColorU32(ImVec4(color)), thickness);
}
void GUI::Draw::QuadFilled(const Math::Vector2& a, const Math::Vector2& b,
                           const Math::Vector2& c, const Math::Vector2& d,
                           Color color) {
  ImGui::GetWindowDrawList()->AddQuadFilled(ImVec2(a), ImVec2(b), ImVec2(c),
                                            ImVec2(d),
                                            ImGui::GetColorU32(ImVec4(color)));
}
void GUI::Draw::Triangle(const Math::Vector2& a, const Math::Vector2& b,
                         const Math::Vector2& c, Color color, float thickness) {
  ImGui::GetWindowDrawList()->AddTriangle(ImVec2(a), ImVec2(b), ImVec2(c),
                                          ImGui::GetColorU32(ImVec4(color)),
                                          thickness);
}
void GUI::Draw::TriangleFilled(const Math::Vector2& a, const Math::Vector2& b,
                               const Math::Vector2& c, Color color) {
  ImGui::GetWindowDrawList()->AddTriangleFilled(
      ImVec2(a), ImVec2(b), ImVec2(c), ImGui::GetColorU32(ImVec4(color)));
}
void GUI::Draw::Circle(const Math::Vector2& center, float radius, Color color,
                       int segments, float thickness) {
  ImGui::GetWindowDrawList()->AddCircle(ImVec2(center), radius,
                                        ImGui::GetColorU32(ImVec4(color)),
                                        segments, thickness);
}
void GUI::Draw::CircleFilled(const Math::Vector2& center, float radius,
                             Color color, int segments) {
  ImGui::GetWindowDrawList()->AddCircleFilled(
      ImVec2(center), radius, ImGui::GetColorU32(ImVec4(color)), segments);
}
void GUI::Image(const Math::Rect& position, const TextureID& textureId,
                const Math::Vector2& size, const Math::Vector2& offset,
                const Math::Vector2& tiling, const Color& tint,
                const Color& border) {
  ImGui::SetCursorPos((ImVec2)position.Position());
  ImGui::Image(textureId, ImVec2(position.Size()), ImVec2(offset),
               ImVec2(tiling), ImVec4(tint), ImVec4(border));
}
void GUI::ProgressBar(const Math::Rect& position, float fraction,
                      const Math::Vector2& size, const std::string& overlay) {
  // TODO(Jacob) Styling
  ImGui::SetCursorPos((ImVec2)position.Position());
  ImGui::ProgressBar(fraction, ImVec2(position.Size()), overlay.c_str());
}

GUIStyle GUI::GetStyle() { return GUIStyle(ImGui::GetStyle()); }
}  // namespace Isetta
