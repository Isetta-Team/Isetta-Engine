/*
 * Copyright (c) 2018 Isetta
 */
#include "Font.h"

#include "GUIModule.h"
#include "imgui_internal.h"

namespace Isetta {
Font* Font::GetDefaultFont() {
  return reinterpret_cast<Font*>(ImGui::GetDefaultFont());
}
void Font::AddDefaultFont(const std::string_view& fontName, float size) {
  Font* font = guiModule->GetFont(fontName, size);
  if (font)
    ImGui::GetIO().Fonts->AddFontDefault(
        reinterpret_cast<ImFont*>(font)->ConfigData);
}
void Font::AddDefaultFont(Font* const font) {
  ImGui::GetIO().FontDefault = reinterpret_cast<ImFont*>(font);
}
Font* Font::GetFont(const std::string_view fontName, float size) {
  return guiModule->GetFont(fontName, size);
}
Font* Font::AddFontFromFile(const std::string& filename, float fontSize,
                            const std::string_view& fontName) {
  if (fontName.empty()) {
    Font* font = guiModule->GetFont(filename, fontSize);
    if (font) return font;
    font = reinterpret_cast<Font*>(
        ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), fontSize));
    if (!font) {
      LOG_ERROR(Debug::Channel::GUI,
                "GUI::AddFontFromFile => Could not load font %s",
                filename.c_str());
      return nullptr;
    }
    guiModule->AddFont(filename, fontSize, font);
    return font;
  } else {
    Font* font = guiModule->GetFont(fontName, fontSize);
    if (font) return font;
    font = reinterpret_cast<Font*>(
        ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), fontSize));
    if (!font) {
      LOG_ERROR(Debug::Channel::GUI,
                "GUI::AddFontFromFile => Could not load font %s",
                filename.c_str());
      return nullptr;
    }
    guiModule->AddFont(fontName, fontSize, font);
    return font;
  }
}
Font* Font::AddFontFromMemory(void* fontBuffer, float fontSize, float pixels,
                              const std::string_view& fontName) {
  auto font = reinterpret_cast<Font*>(
      ImGui::GetIO().Fonts->AddFontFromMemoryTTF(fontBuffer, fontSize, pixels));
  if (fontName.empty()) guiModule->AddFont(fontName, fontSize, font);
  return font;
}
void Font::PushFont(const std::string_view fontName, float fontSize) {
  Font* font = guiModule->GetFont(fontName, fontSize);
  if (font) ImGui::PushFont(reinterpret_cast<ImFont*>(font));
}
void Font::PushFont(Font* const font) {
  ImGui::PushFont(reinterpret_cast<ImFont*>(font));
}
void Font::PopFont() { ImGui::PopFont(); }
}  // namespace Isetta