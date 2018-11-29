/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Core/Math/Vector2Int.h"
#include "ISETTA_API.h"

namespace Isetta::Math {
class Rect;
}

namespace Isetta {
class ISETTA_API Window {
 private:
  static class WindowModule *windowModule;
  friend class WindowModule;

 public:
  static int GetWidth();
  static int GetHeight();

  // static void SetIcon(const class Texture &icon);
  static void SetTitle(const std::string_view title);
  /**
   * @brief Set the Size Limits object
   *
   * @param size the minimum (x,y) and maximum (width, height) size limits, set
   * to -1 if there is no limit in that direction
   */
  static void SetSizeLimits(const Math::Rect &size);
  static void SetFullScreen();
};
}  // namespace Isetta
