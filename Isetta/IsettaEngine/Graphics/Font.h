/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_map>
#include "Util.h"
#include "imgui/imgui.h"

class StringId;

namespace Isetta {
struct ISETTA_API Font : private ImFont {
  static Font* GetDefaultFont();
  static void AddDefaultFont(const std::string_view& fontName, float size);
  static void AddDefaultFont(Font* const font);
  static Font* GetFont(const std::string_view fontName, float size);
  static Font* AddFontFromFile(const std::string& filename, float fontSize,
                               const std::string_view& fontName = "");
  static Font* AddFontFromMemory(void* fontBuffer, float fontSize, float pixels,
                                 const std::string_view& fontName);
  static void PushFont(const std::string_view fontName, float fontSize);
  static void PushFont(Font* const font);
  static void PopFont();

 private:
  static void AddFontToMap(const std::string_view fontName, float size,
                           Font* const font);
  static std::unordered_map<std::pair<StringId, float>, Font*, Util::PairHash>
      fonts;

  static class GUIModule* guiModule;
  friend class GUIModule;
  friend class GUI;
};
}  // namespace Isetta
