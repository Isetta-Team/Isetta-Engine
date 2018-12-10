/*
 * Copyright (c) 2018 Isetta
 */
#include "FrameReporter.h"

#include "Core/Time/Time.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Util.h"

namespace Isetta {
#ifdef _EDITOR
void FrameReporter::GuiUpdate() {
  ++count;
  float dt = Time::GetDeltaTime();

  if (count >= reportInterval) {
    fps = 1.0f / dt;
    frameTime = dt * 1000;
    count = 0;
  }

  GUI::Window(RectTransform{Math::Rect{20, 10, 150, 80}}, "Frame Reporter",
              [&]() {
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

                GUI::Text(RectTransform{Math::Rect{5, 25, 50, 5}},
                          Util::StrFormat("Avg: %.1f ms", timeSumForAvg * 1000 /
                                                              frameCountForAvg),
                          style);
              },
              &isOpen);
}
void FrameReporter::Open() { isOpen = true; }
#endif
}  // namespace Isetta
