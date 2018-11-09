/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/W10UIManager.h"
#include "Events/Events.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Input/Input.h"

void W10UIManager::Awake() {
  Isetta::Events::Instance().RegisterEventListener(
      "Spawn",
      [&](const Isetta::EventObject obj) { displayText = "Start game!"; });
}

void W10UIManager::GuiUpdate() {
  Isetta::GUI::Text(Isetta::RectTransform{Isetta::Math::Rect{0, -200, 100, 100},
                                          Isetta::GUI::Pivot::Center,
                                          Isetta::GUI::Pivot::Center},
                    displayText);
}
