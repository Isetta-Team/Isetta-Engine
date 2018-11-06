/*
 * Copyright (c) 2018 Isetta
 */
#include "GUILevel/GUIComponent.h"

#include "Core/Color.h"
#include "Core/Debug/Logger.h"
#include "Core/Time/Clock.h"
#include "EngineLoop.h"
#include "Graphics/GUI.h"
#include "Graphics/RectTransform.h"
#include "Util.h"
#include "imgui/imgui.h"

using namespace Isetta;

bool GUIComponent::checkbox = false;
void GUIComponent::GuiUpdate() {
  // GUI Test
  GUI::Button(
      RectTransform{Math::Rect{0, 0, 80, 20}, GUI::Pivot::Bot, GUI::Pivot::Bot},
      "btn", []() { LOG_INFO(Debug::Channel::GUI, "btn"); },
      GUI::ButtonStyle{Color::red, Color::blue, Color::yellow});
  // GUI::ButtonImage(RectTransform{{100, 40, 80, 20}}, "btn-id", NULL,
  //                  []() { LOG_INFO(Debug::Channel::GUI, "btn image"); },
  //                  GUI::ButtonStyle{Color::grey, Color::green, Color::cyan},
  //                  GUI::ImageStyle{Color::blue, Color::white, 2}, true);
  GUI::Toggle(RectTransform{{10, 130, 40, 40}}, "toggle me", &checkbox,
              GUI::ButtonStyle{Color::red, Color::blue, Color::yellow});
  GUI::Text(RectTransform{{100, 130, 40, 40}},
            Util::StrFormat("I am %s and I am %d", "Jake", 10),
            GUI::TextStyle{false, false, Color::grey});
  GUI::Label(RectTransform{{100, 230, 40, 40}}, "labelthing", "text",
             GUI::LabelStyle{Color::white, Color::clear});

  static char buffer[1024];
  // struct TestCallback {
  //  static int Callback(GUIInputTextCallbackData* data) {
  //    LOG_INFO(Debug::Channel::GUI, data->Buf);
  //    return 0;
  //  }
  //};
  // GUI::InputText(RectTransform{{100, 150, 80, 40}}, "inputText", buffer,
  // 1024,
  //               GUI::InputStyle{},
  //               GUI::InputTextFlags::CallbackCompletion |
  //                   GUI::InputTextFlags::EnterReturnsTrue,
  //               TestCallback::Callback);
  static int val = 0;
  GUI::InputInt(RectTransform{{1000, 150, 100, 40}}, "inputInt", &val);
  static bool open = false;
  open = open || GUI::Button(RectTransform{{100, 700, 80, 20}}, "window btn");
  static RectTransform movableWindow{{200, 700, 500, 200}};
  if (open) {
    GUI::Window(movableWindow, "window name",
                []() {
                  GUI::MenuBar([]() {
                    ImGui::Text("words in menu");
                    GUI::Menu("menu2", []() {
                      GUI::MenuItem("item2", "Ctrl+99", []() {
                        LOG_INFO(Debug::Channel::GUI, "menu selected");
                      });
                    });
                    GUI::Menu("menu disabled",
                              []() { GUI::MenuItem("item3", "Ctrl+99"); },
                              false);
                  });
                  ImGui::Text("words in window");
                },
                &open, GUI::WindowStyle{}, GUI::WindowFlags::MenuBar);
  }
  GUI::MenuBar(
      []() { GUI::Menu("menu1", []() { GUI::MenuItem("item1", "Ctrl+99"); }); },
      true);

  GUI::Modal(
      RectTransform{{0, 0, 100, 100}, GUI::Pivot::Center, GUI::Pivot::Center},
      "modal", []() {
        if (ImGui::Button("close", (ImVec2)Math::Vector2(40, 40))) {
          GUI::CloseCurrentPopup();
        }
      });
  if (GUI::Button(RectTransform{{100, 800, 80, 20}}, "modal btn")) {
    GUI::OpenPopup("modal");
  }

  GUI::Draw::Rect(RectTransform{{300, 300, 100, 100}}, Color::blue, 0.1f,
                  GUI::DrawCornerFlags::BotRight, 2.0f);
  GUI::Draw::RectFilled(RectTransform{{300, 300, 10, 150}}, Color::red);

  GUI::Draw::Quad(RectTransform{{0, 0, 0, 0}}, Math::Vector2{600, 30},
                  Math::Vector2{630, 60}, Math::Vector2{630, 90},
                  Math::Vector2{610, 70}, Color::white, 1.5f);
  GUI::Draw::QuadFilled(RectTransform{{0, 0, 0, 0}}, Math::Vector2{600, 130},
                        Math::Vector2{630, 160}, Math::Vector2{630, 190},
                        Math::Vector2{610, 170}, Color::grey);

  GUI::Draw::Triangle(RectTransform{{0, 0, 0, 0}}, Math::Vector2{400, 400},
                      Math::Vector2(440, 440), Math::Vector2(400, 480),
                      Color::green, 3.0f);
  GUI::Draw::TriangleFilled(RectTransform{{0, 0, 0, 0}},
                            Math::Vector2{480, 400}, Math::Vector2{440, 440},
                            Math::Vector2{480, 480}, Color::cyan);

  GUI::Draw::Circle(RectTransform{{530, 530, 0, 0}}, 10, Color::yellow, 6, .1f);

  GUI::Draw::CircleFilled(RectTransform{{530, 80, 0, 0}}, 30, Color::blue, 14);
  GUI::Draw::CircleFilled(RectTransform{{530, 80, 0, 0}}, 10, Color::black);

  // GUI::Image(const Math::Rect& position, const TextureID&
  // textureId, const Math::Vector2& size, const Math::Vector2&
  // offset = Math::Vector2::zero, const Math::Vector2& tiling =
  //    Math::Vector2::one,  // TODO(Jacob) what is uv used for?
  // const Color& tint = Color::black,
  // const Color& border = Color::clear);
  static float progress = 0.0f, progressDir = 1.0f;
  if (true) {
    progress += progressDir * 0.4f * EngineLoop::GetGameClock().GetDeltaTime();
    if (progress >= +1.1f) {
      progress = +1.1f;
      progressDir *= -1.0f;
    }
    if (progress <= -0.1f) {
      progress = -0.1f;
      progressDir *= -1.0f;
    }
  }
  GUI::ProgressBar(RectTransform{{700, 700, 100, 30}}, progress);
  float progressSaturated =
      (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
  GUI::ProgressBar(
      RectTransform{{700, 740, 100, 100}}, progress,
      Util::StrFormat("%d/%d", static_cast<int>((312 * progressSaturated)),
                      312));
}
