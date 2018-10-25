/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/FrameReporter.h"
#include "IsettaCore.h"

namespace Isetta {

void FrameReporter::GuiUpdate() {
  count++;
  totalFrameCount++;
  if (count >= reportInterval) {
    float dt = Time::GetDeltaTime();
    fps = 1.0f / dt;
    frameTime = dt * 1000;
    count = 0;
  }

  GUI::TextStyle style{Color::white};

  GUI::Text(RectTransform{Math::Rect{5, 5, 50, 5}},
            Util::StrFormat("FPS: %.0f", fps), style);

  GUI::Text(RectTransform{Math::Rect{5, 15, 50, 5}},
            Util::StrFormat("Time: %.1f ms", frameTime), style);

  GUI::Text(
      RectTransform{Math::Rect{5, 25, 50, 5}},
      Util::StrFormat("Avg: %.1f ms", Time::GetElapsedTime() * 1000 / totalFrameCount),
      style);
}
}  // namespace Isetta
