/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/FrameReporter.h"
#include "IsettaCore.h"

namespace Isetta {

void FrameReporter::GuiUpdate() {
  count++;
  float dt = Time::GetDeltaTime();

  if (count >= reportInterval) {
    fps = 1.0f / dt;
    frameTime = dt * 1000;
    count = 0;
  }

  GUI::TextStyle style{Color::white};

  GUI::Text(RectTransform{Math::Rect{5, 5, 50, 5}},
            Util::StrFormat("FPS: %.0f", fps), style);

  GUI::Text(RectTransform{Math::Rect{5, 15, 50, 5}},
            Util::StrFormat("Time: %.1f ms", frameTime), style);

  frameDurations.push(dt);
  timeSumForAvg += dt;
  if (frameDurations.size() > frameCountForAvg) {
    timeSumForAvg -= frameDurations.front();
    frameDurations.pop();
  }

  GUI::Text(
      RectTransform{Math::Rect{5, 25, 50, 5}},
      Util::StrFormat("Avg: %.1f ms", timeSumForAvg * 1000 / frameCountForAvg),
      style);
}
}  // namespace Isetta
