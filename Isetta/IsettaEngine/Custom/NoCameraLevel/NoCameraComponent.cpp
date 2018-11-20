/*
 * Copyright (c) 2018 Isetta
 */
#include "NoCameraComponent.h"

#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"

namespace Isetta {
void NoCameraComponent::GuiUpdate() {
  const RectTransform rect{
      {0, 0, 0, 0}, GUI::Pivot::Center, GUI::Pivot::Center};
  GUI::Text(rect, "No cameras rendering: Load level with camera.");
}
}  // namespace Isetta
