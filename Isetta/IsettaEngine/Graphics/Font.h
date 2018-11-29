/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <initializer_list>
#include <stack>
#include <string>
#include <unordered_map>
#include "Core/DataStructures/Array.h"
#include "Util.h"
#include "imgui/imgui.h"

class StringId;

namespace Isetta {
struct ISETTA_API Font : private ImFont {
  static Font* GetDefaultFont();
  static void AddDefaultFont(const std::string_view& fontName, float size);
  static void AddDefaultFont(Font* const font);
  static Font* GetFont(const std::string_view fontName, float size);
  static void AddFontFromFile(const std::string_view& filename, float fontSize,
                              const std::string_view& fontName);
  static void AddFontFromFile(const std::string_view& filename,
                              std::initializer_list<float> fontSizes,
                              const std::string_view& fontName);
  static void AddFontFromFile(const std::string_view& filename,
                              const std::string_view& fontName);
  static Font* AddFontFromMemory(void* fontBuffer, float fontSize, float pixels,
                                 const std::string_view& filename,
                                 const std::string_view& fontName);
  static void PushFont(const std::string_view fontName, float fontSize);
  static void PushFont(Font* const font);
  static void PopFont();

 private:
  // Name -> <filepath, size -> Font>
  static std::unordered_map<
      StringId, std::pair<std::string, std::unordered_map<float, Font*>>>
      fonts;
  static std::stack<std::tuple<StringId, std::string, float>> loadFonts;

  static class GUIModule* guiModule;
  friend class GUIModule;
  friend class GUI;
};
}  // namespace Isetta
