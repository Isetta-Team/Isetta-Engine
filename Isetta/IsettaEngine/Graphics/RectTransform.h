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
ISETTA_API struct RectTransform {
  Math::Rect rect;
  GUI::Pivot anchor = GUI::Pivot::TopLeft;
  GUI::Pivot pivot = GUI::Pivot::TopLeft;
  RectTransform() = default;
  explicit RectTransform(const Math::Rect &rect) : rect{rect} {}
  RectTransform(const Math::Rect &rect, GUI::Pivot anchor)
      : rect{rect}, anchor{anchor} {}
  RectTransform(const Math::Rect &rect, GUI::Pivot anchor, GUI::Pivot pivot)
      : rect{rect}, anchor{anchor}, pivot{pivot} {}

  RectTransform(const RectTransform &transform)
      : rect{transform.rect},
        anchor{transform.anchor},
        pivot{transform.pivot} {}
  RectTransform(RectTransform &&transform) noexcept
      : anchor{transform.anchor}, pivot{transform.pivot} {
    rect = std::move(transform.rect);
  }
  inline RectTransform &operator=(const RectTransform &transform) {
    rect = transform.rect;
    anchor = transform.anchor;
    pivot = transform.pivot;
    return *this;
  }
  inline RectTransform &operator=(RectTransform &&transform) noexcept {
    rect = std::move(transform.rect);
    anchor = transform.anchor;
    pivot = transform.pivot;
    return *this;
  }
};
}  // namespace Isetta
