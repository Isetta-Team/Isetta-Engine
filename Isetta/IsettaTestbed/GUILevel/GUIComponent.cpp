/*
 * Copyright (c) 2018 Isetta
 */
#include "GUILevel/GUIComponent.h"
#include "imgui/imgui.h"

using namespace Isetta;

bool GUIComponent::checkbox = false;
void GUIComponent::GuiUpdate() {
  // Button
  //  RectTransform(Rect(position, size), Window Pivot, GUI Item Pivot)
  //  Label of button
  //  Button callback when clicked
  //  ButtonStyle(background, hover, active) colors
  //    see GUI::ButtonStyle for more constructor arguments
  GUI::Button(
      RectTransform{Math::Rect{0, 0, 80, 20}, GUI::Pivot::Bot, GUI::Pivot::Bot},
      "btn", []() { LOG_INFO(Debug::Channel::GUI, "btn"); },
      GUI::ButtonStyle{Color::red, Color::blue, Color::yellow});

  // Image as Button
  // GUI::ButtonImage(RectTransform{{100, 40, 80, 20}}, "btn-id", NULL,
  //                  []() { LOG_INFO(Debug::Channel::GUI, "btn image"); },
  //                  GUI::ButtonStyle{Color::grey, Color::green, Color::cyan},
  //                  GUI::ImageStyle{Color::blue, Color::white, 2}, true);

  // Toggle/Checkbox
  GUI::Toggle(RectTransform{{10, 130, 40, 40}}, "toggle me", &checkbox,
              GUI::ButtonStyle{Color::red, Color::blue, Color::yellow});

  // Text
  //  set Rect width/height to 0 to match size of text
  //  Util::StrFormat formats a string with parameters like printf
  //  TextStyle(wrapped, disabled, textColor)
  //    see GUI::TextStyle for more constructor arguments
  GUI::Text(RectTransform{{100, 130, 40, 40}},
            Util::StrFormat("I am %s and I am %d", "Jake", 10),
            GUI::TextStyle{false, false, Color::grey});

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

  // Input field for only int
  static int val = 0;
  GUI::InputInt(RectTransform{{1000, 150, 100, 40}}, "inputInt", &val);

  static bool open = false;
  open = open || GUI::Button(RectTransform{{100, 700, 80, 20}}, "window btn");
  static RectTransform movableWindow{{200, 700, 500, 200}};
  if (open) {
    // Window
    //  GUI code in callback displays in window
    GUI::Window(movableWindow, "window name",
                []() {
                  // Menu bar for specific window
                  GUI::MenuBar([]() {
                    // Use of ImGui in engine
                    ImGui::Text("words in menu");

                    // Menu dropdown
                    GUI::Menu("menu2", []() {
                      // Item in menu dropdown & callback when clicked
                      //  Shortcut doesn't automatically register to Input
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

  // Menu bar for the full window
  GUI::MenuBar(
      []() { GUI::Menu("menu1", []() { GUI::MenuItem("item1", "Ctrl+99"); }); },
      true);

  // Modal window
  //  Cannot click anyother element while up
  GUI::Modal(
      RectTransform{{0, 0, 100, 100}, GUI::Pivot::Center, GUI::Pivot::Center},
      "modal", []() {
        if (ImGui::Button("close", (ImVec2)Math::Vector2(40, 40))) {
          // Closes the popup
          GUI::CloseCurrentPopup();
        }
      });

  if (GUI::Button(RectTransform{{100, 800, 80, 20}}, "modal btn")) {
    // Opens popup
    GUI::OpenPopup("modal");
  }

  /// Shape Drawings
  // Rect: outline of rectangle
  GUI::Draw::Rect(RectTransform{{300, 300, 100, 100}}, Color::blue, 0.1f,
                  GUI::DrawCornerFlags::BotRight, 2.0f);

  // RectFilled: filled in rectangle
  GUI::Draw::RectFilled(RectTransform{{300, 300, 10, 150}}, Color::red);

  // Quad: outline polygon centered at RectTransform with verticies
  GUI::Draw::Quad(RectTransform{{0, 0, 0, 0}}, Math::Vector2{600, 30},
                  Math::Vector2{630, 60}, Math::Vector2{630, 90},
                  Math::Vector2{610, 70}, Color::white, 1.5f);

  // Quad: filled version
  GUI::Draw::QuadFilled(RectTransform{{0, 0, 0, 0}}, Math::Vector2{600, 130},
                        Math::Vector2{630, 160}, Math::Vector2{630, 190},
                        Math::Vector2{610, 170}, Color::grey);

  // Triangle: outline of triangle centered at RectTransform
  GUI::Draw::Triangle(RectTransform{{0, 0, 0, 0}}, Math::Vector2{400, 400},
                      Math::Vector2(440, 440), Math::Vector2(400, 480),
                      Color::green, 3.0f);

  // Triangle: filled version
  GUI::Draw::TriangleFilled(RectTransform{{0, 0, 0, 0}},
                            Math::Vector2{480, 400}, Math::Vector2{440, 440},
                            Math::Vector2{480, 480}, Color::cyan);

  // Circle: outline of circle centered at RectTransform
  GUI::Draw::Circle(RectTransform{{530, 530, 0, 0}}, 10, Color::yellow, 6, .1f);

  // Circle: filled version
  GUI::Draw::CircleFilled(RectTransform{{530, 80, 0, 0}}, 30, Color::blue, 14);
  GUI::Draw::CircleFilled(RectTransform{{530, 80, 0, 0}}, 10, Color::black);

  // Loading Image
  // GUI::Image(const Math::Rect& position, const Texture&
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

  // Progress bar (like loading bar)
  GUI::ProgressBar(RectTransform{{700, 700, 100, 30}}, progress);
  float progressSaturated =
      (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
  GUI::ProgressBar(
      RectTransform{{700, 740, 100, 100}}, progress,
      Util::StrFormat("%d/%d", static_cast<int>((312 * progressSaturated)),
                      312));
}
