/*
 * Copyright (c) 2018 Isetta
 */
#include "Font.h"

#include "Core/Config/Config.h"
#include "GUIModule.h"
#include "imgui_internal.h"

namespace Isetta {
GUIModule* Font::guiModule;
std::unordered_map<std::pair<StringId, float>, Font*, Util::PairHash>
    Font::fonts;

Font* Font::GetDefaultFont() {
  return reinterpret_cast<Font*>(ImGui::GetDefaultFont());
}
void Font::AddDefaultFont(const std::string_view& fontName, float size) {
  Font* font = GetFont(fontName, size);
  if (font)
    ImGui::GetIO().Fonts->AddFontDefault(
        reinterpret_cast<ImFont*>(font)->ConfigData);
}
void Font::AddDefaultFont(Font* const font) {
  ImGui::GetIO().FontDefault = reinterpret_cast<ImFont*>(font);
}
Font* Font::GetFont(const std::string_view fontName, float size) {
  auto font = fonts.find({SID(fontName.data()), size});
  return font == fonts.end() ? nullptr : font->second;
}
Font* Font::AddFontFromFile(const std::string& fileName, float fontSize,
                            const std::string_view& fontName) {
  const std::string filePath = CONFIG_VAL(resourcePath) + R"(\)" + fileName;
  if (fontName.empty()) {
    Font* font = GetFont(fileName, fontSize);
    if (font) return font;
    font = reinterpret_cast<Font*>(
        ImGui::GetIO().Fonts->AddFontFromFileTTF(filePath.c_str(), fontSize));
    if (!font) {
      LOG_ERROR(Debug::Channel::GUI,
                "GUI::AddFontFromFile => Could not load font %s",
                fileName.c_str());
      return nullptr;
    }
    AddFontToMap(fileName, fontSize, font);
    return font;
  } else {
    Font* font = GetFont(fontName, fontSize);
    if (font) return font;
    font = reinterpret_cast<Font*>(
        ImGui::GetIO().Fonts->AddFontFromFileTTF(filePath.c_str(), fontSize));
    if (!font) {
      LOG_ERROR(Debug::Channel::GUI,
                "GUI::AddFontFromFile => Could not load font %s",
                fileName.c_str());
      return nullptr;
    }
    AddFontToMap(fontName, fontSize, font);
    return font;
  }
}
Font* Font::AddFontFromMemory(void* fontBuffer, float fontSize, float pixels,
                              const std::string_view& fontName) {
  auto font = reinterpret_cast<Font*>(
      ImGui::GetIO().Fonts->AddFontFromMemoryTTF(fontBuffer, fontSize, pixels));
  if (fontName.empty()) AddFontToMap(fontName, fontSize, font);
  return font;
}
void Font::PushFont(const std::string_view fontName, float fontSize) {
  Font* font = GetFont(fontName, fontSize);
  if (font) ImGui::PushFont(reinterpret_cast<ImFont*>(font));
}
void Font::PushFont(Font* const font) {
  ImGui::PushFont(reinterpret_cast<ImFont*>(font));
}
void Font::PopFont() { ImGui::PopFont(); }

void Font::AddFontToMap(const std::string_view fontName, float size,
                        Font* const font) {
  fonts.insert({{SID(fontName.data()), size}, font});
}
}  // namespace Isetta