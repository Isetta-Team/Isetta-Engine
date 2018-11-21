/*
 * Copyright (c) 2018 Isetta
 */
#include "PauseComponent.h"

#include "Core/Math/Vector2.h"
#include "Core/Math/Vector2Int.h"
#include "Core/Time/Time.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Input/Input.h"

namespace Isetta {
void PauseComponent::OnEnable() {
  handle = Input::RegisterKeyPressCallback(KeyCode::P, [&]() {
    pause = !pause;
    if (pause)
      // Time::Pause();
      Time::SetGameScale(0);
    else
      Time::SetGameScale(1);
    // Time::Resume();
  });
}

void PauseComponent::OnDisable() {
  Input::UnregisterKeyPressCallback(KeyCode::P, handle);
}

void PauseComponent::GuiUpdate() {
  if (!pause) return;

  const float offset = 30;
  Math::Vector2 winSize = static_cast<Math::Vector2>(GUI::GetWindowSize(true));
  GUI::Draw::Rect(RectTransform{{offset, offset, winSize.x - 2 * offset,
                                 winSize.y - 2 * offset}},
                  border, 2.f);

  const float width = 100, height = 250;
  GUI::Draw::RectFilled(RectTransform{{-.75f * width, 0, width, height},
                                      GUI::Pivot::Center,
                                      GUI::Pivot::Center},
                        border);
  GUI::Draw::RectFilled(RectTransform{{.75f * width, 0, width, height},
                                      GUI::Pivot::Center,
                                      GUI::Pivot::Center},
                        border);
}
void PauseComponent::Update() {}
}  // namespace Isetta
