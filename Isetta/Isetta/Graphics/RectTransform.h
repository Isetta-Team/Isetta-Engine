/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/Math/Rect.h"
#include "Graphics/GUI.h"

namespace Isetta::Math {
class Rect;
class Vector2;
}  // namespace Isetta::Math

namespace Isetta {
struct RectTransform {
  Math::Rect rect;
  GUI::Pivot anchor = GUI::Pivot::TopLeft;
  GUI::Pivot pivot = GUI::Pivot::TopLeft;
  RectTransform() = default;
  RectTransform(const Math::Rect& rect) : rect{rect} {}
  RectTransform(const Math::Rect& rect, GUI::Pivot anchor)
      : rect{rect}, anchor{anchor} {}
  RectTransform(const Math::Rect& rect, GUI::Pivot anchor, GUI::Pivot pivot)
      : rect{rect}, anchor{anchor}, pivot{pivot} {}
};
}  // namespace Isetta
