/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/W10UIManager.h"

void W10UIManager::Awake() {
  Isetta::Events::Instance().RegisterEventListener(
      "UITextChange", [&](const Isetta::EventObject obj) {
        displayText = std::get<std::string>(obj.eventParams[0]);
      });
  Isetta::Events::Instance().RegisterEventListener("Win",
                                                   [&](auto) { ++winning; });
  Isetta::Events::Instance().RegisterEventListener("Lose",
                                                   [&](auto) { ++losing; });
}

void W10UIManager::GuiUpdate() {
  Isetta::GUI::Text(
      Isetta::RectTransform{Isetta::Math::Rect{50, 50, 100, 100},
                            Isetta::GUI::Pivot::TopLeft,
                            Isetta::GUI::Pivot::TopLeft},
      Isetta::Util::StrFormat("Win: %d\nLose: %d", winning, losing));
  Isetta::GUI::Text(Isetta::RectTransform{Isetta::Math::Rect{0, -200, 100, 100},
                                          Isetta::GUI::Pivot::Center,
                                          Isetta::GUI::Pivot::Center},
                    displayText);
}
