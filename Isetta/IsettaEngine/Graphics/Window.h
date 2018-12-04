/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Core/Math/Vector2Int.h"
#include "ISETTA_API.h"

namespace Isetta::Math {
class Rect;
class Vector2Int;
}  // namespace Isetta::Math

namespace Isetta {
class ISETTA_API Window {
 private:
  static class WindowModule *windowModule;
  friend class WindowModule;

 public:
  static int GetWidth();
  static int GetHeight();

  static void SetIcon(const class Texture &icon);
  static void SetTitle(const std::string_view title);
  /**
   * @brief Set the Size Limits object
   *
   * @param size the minimum (x,y) and maximum (width, height) size limits, set
   * to -1 if there is no limit in that direction
   */
  static void SetSizeLimits(const Math::Rect &size);
  static void SetFullscreen(bool fullscreen = true);
  static bool IsFullscreen();

  enum class Cursor {
    None = -1,
    Arrow = 0,
    IBeam = 1,
    VResize = 3,
    HResize = 4,
    Hand = 7,
    Crosshair = 5,
    Custom = 6,
  };
  enum class CursorMode {
    Normal,
    Hidden,
    Disabled,
  };
  static void SetCursorIcon(const Cursor icon);
  static void SetCursorIcon(
      const class Texture &icon,
      const Math::Vector2Int &hotspot = Math::Vector2Int::zero);
  static void SetCursorMode(const CursorMode &mode);
};
}  // namespace Isetta
