/*
 * Copyright (c) 2018 Isetta
 */
#include "ScoreManager.h"
#include "KnightGame/Constants.h"

using namespace Isetta;

namespace KnightGame {
void ScoreManager::Start() {
  if (!instance) instance = this;
  Font::AddFontFromFile("KnightGame\\Fonts\\KnightsQuest.ttf", 72.f,
                        "KnightsQuest");
  Font::AddFontFromFile("KnightGame\\Fonts\\KnightsQuest.ttf", 42.f,
                        "KnightsQuest");
}

void ScoreManager::OnEnable() {
  gameOverEvent = Events::Instance().RegisterEventListener(
      GAMEOVER_EVENT,
      [&](const Isetta::EventObject& eventObject) { gameOver = true; });
}

void ScoreManager::OnDisable() {
  Events::Instance().UnregisterEventListener(GAMEOVER_EVENT, gameOverEvent);
}

void ScoreManager::GuiUpdate() {
  if (!titleFont) titleFont = Font::GetFont("KnightsQuest", 72.f);
  if (!scoreFont) scoreFont = Font::GetFont("KnightsQuest", 42.f);

  GUI::Text(RectTransform{{0, 100, 0, 0}, GUI::Pivot::Top, GUI::Pivot::Top},
            "KNIGHT'S CONQUEST", GUI::TextStyle{titleFont});
  const float initX = -120, initY = 50;
  RectTransform rect{
      {initX, initY, 0, 0}, GUI::Pivot::TopRight, GUI::Pivot::Center};
  if (gameOver) {
    elapsed += speed * Time::GetDeltaTime();
    rect.rect.x = Math::Util::Lerp(initX, -0.5f * Window::GetWidth(), elapsed);
    rect.rect.y =
        Math::Util::Lerp(initY, 0.5f * Window::GetHeight() - 100, elapsed);
  }
  GUI::Text(rect, Util::StrFormat("Score: %03d", score),
            GUI::TextStyle{scoreFont});
}  // namespace KnightGame
}  // namespace KnightGame
