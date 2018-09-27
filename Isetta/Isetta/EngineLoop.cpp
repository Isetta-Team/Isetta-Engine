/*
 * Copyright (c) 2018 Isetta
 */
#include "EngineLoop.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/ModuleManager.h"
#include "Core/Time/Clock.h"
#include "Graphics/AnimationNode.h"
#include "Graphics/GUI.h"
#include "Graphics/LightNode.h"
#include "Graphics/ModelNode.h"
#include "Input/Input.h"
#include "Input/InputEnum.h"
#include "Networking/NetworkManager.h"

#include "Core/Color.h"
#include "Core/Math/Rect.h"
#include "Graphics/GUIStyle.h"
#include "imgui.h"

// TODO(Jacob) remove
bool checkbox = false;
namespace Isetta {

void RunYidiTest();

void EngineLoop::StartUp() {
  Logger::NewSession();
  Config::Instance().Read("config.cfg");

  intervalTime = 1.0 / Config::Instance().loopConfig.maxFps.GetVal();
  maxSimulationCount = Config::Instance().loopConfig.maxSimCount.GetVal();

  moduleManager.StartUp();

  StartGameClock();
  isGameRunning = true;

  Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                  [&]() { isGameRunning = false; });

  // Game Init Part

  // Networking
  if (Config::Instance().networkConfig.runServer.GetVal()) {
    NetworkManager::CreateServer(
        Config::Instance().networkConfig.defaultServerIP.GetVal().c_str());
  }
  if (Config::Instance().networkConfig.connectToServer.GetVal()) {
    NetworkManager::ConnectToServer(
        Config::Instance().networkConfig.defaultServerIP.GetVal().c_str(),
        [](bool b) {
          LOG(Debug::Channel::Networking, "Client connection state: %d", b);
        });
  }

  // TODO(All) Read scene from scene file
  // ModelNode car{"test/Low-Poly-Racing-Car.scene.xml", Math::Vector3{0, -20,
  // 0},
  //              Math::Vector3::zero, Math::Vector3::one};
  static ModelNode car{"push/Pushing.scene.xml", Math::Vector3{-200, -100, 0},
                       Math::Vector3{0, 90, 0}, Math::Vector3::one};
  static AnimationNode animation{&car};
  animation.AddAnimation("push/Pushing.anim", 0, "", false);
  animation.Play();
  LightNode light{"materials/light.material.xml", Math::Vector3{0, 200, 600},
                  Math::Vector3::zero, Math::Vector3::one};

  // TODO(Chaojie) remove later into game logic

  // Register Input callback from player script
  U64 handleA, handleB, handleC;
  handleA = Input::RegisterKeyPressCallback(KeyCode::NUM1, [&handleA]() {
    LOG_INFO(Debug::Channel::General, "A pressed");
    Input::UnregisterKeyPressCallback(KeyCode::A, handleA);
  });
  handleB = Input::RegisterKeyReleaseCallback(KeyCode::A, [&handleB]() {
    LOG_INFO(Debug::Channel::General, "A released");
    Input::UnregisterKeyReleaseCallback(KeyCode::A, handleB);
  });
  handleC = Input::RegisterMousePressCallback(
      MouseButtonCode::MOUSE_LEFT, [&handleC]() {
        LOG_INFO(Debug::Channel::General,
                 {"Left pressed at: " + Input::GetMousePosition().ToString()});
        Input::UnregisterMousePressCallback(MouseButtonCode::MOUSE_LEFT,
                                            handleC);
      });

  Input::RegisterKeyPressCallback(KeyCode::P, []() {
    if (NetworkManager::ClientIsConnected()) {
      NetworkManager::SendHandleMessageFromClient(0);
    }
  });
  Input::RegisterKeyPressCallback(KeyCode::O, []() {
    if (NetworkManager::ClientIsConnected()) {
      NetworkManager::SendHandleMessageFromClient(1);
    }
  });
  Input::RegisterMousePressCallback(MouseButtonCode::MOUSE_LEFT, []() {
    if (NetworkManager::ClientIsConnected()) {
      NetworkManager::SendHandleMessageFromClient(2);
    }
  });

  // GUI Test
  GUI::OnUpdate([&]() {
    GUI::Button(Math::Rect(0, 0, 80, 20), "btn",
                []() { LOG_INFO(Debug::Channel::GUI, "btn"); },
                GUI::ButtonStyle{Color::red, Color::blue, Color::yellow});
    GUI::ButtonImage(Math::Rect(100, 10, 80, 20), "btn-id", NULL,
                     []() { LOG_INFO(Debug::Channel::GUI, "btn image"); },
                     GUI::ButtonStyle{Color::grey, Color::green, Color::cyan},
                     true, Math::Vector2(0, 0), Math::Vector2(1, 1), 2,
                     Color::blue, Color::white);
    GUI::Toggle(Math::Rect(10, 100, 40, 40), "toggle me", &checkbox,
                GUI::ButtonStyle{Color::red, Color::blue, Color::yellow});
    GUI::Text(Math::Rect(100, 100, 40, 40),
              GUI::TextStyle{false, false, Color::grey},
              "I am Jake and I am %d", 10);
    GUI::Label(Math::Rect(100, 200, 40, 40), "labelthing", Color::white,
               "text");

    static char buffer[1024];
    struct TestCallback {
      static int Callback(GUIInputTextCallbackData* data) {
        LOG_INFO(Debug::Channel::GUI, data->Buf);
        return 0;
      }
    };
    GUI::InputText(Math::Rect(100, 150, 40, 40), "inputtxt", buffer, 1024,
                   GUIInputTextFlags::CallbackCompletion |
                       GUIInputTextFlags::EnterReturnsTrue,
                   TestCallback::Callback);
    // GUI::InputInt(
    // GUI::InputFloat(
    // GUI::Window(
    //
  });

  // RunYidiTest();
}

void EngineLoop::Update() {
  GetGameClock().UpdateTime();

  // TODO(All) Add networking update

  static bool sIsPressed = false;
  static bool cIsPressed = false;

  // Networking update
  if (Input::IsKeyPressed(KeyCode::S)) {
    if (!sIsPressed) {
      sIsPressed = true;
      if (NetworkManager::ServerIsRunning()) {
        for (int i = 0; i < NetworkManager::GetMaxClients(); i++) {
          NetworkManager::SendStringMessageFromServer(i, "Hi!");
        }
      }
    }
  } else {
    sIsPressed = false;
  }
  if (Input::IsKeyPressed(KeyCode::C)) {
    if (!cIsPressed) {
      cIsPressed = true;
      if (NetworkManager::ClientIsConnected()) {
        NetworkManager::SendStringMessageFromClient("Hi!");
      }
    }
  } else {
    cIsPressed = false;
  }

  // end Networking update

  // Client part
  accumulateTime += GetGameClock().GetDeltaTime();

  for (int i = 0; i < maxSimulationCount && accumulateTime > intervalTime;
       i++) {
    moduleManager.SimulationUpdate(intervalTime);
    // LOG_INFO(Debug::Channel::General, std::to_string(accumulateTime));
    accumulateTime -= intervalTime;
  }

  // TODO(Chaojie) after scenegraph, save previous state for prediction
  moduleManager.RenderUpdate(0);
}

void EngineLoop::ShutDown() { moduleManager.ShutDown(); }
void EngineLoop::StartGameClock() const { GetGameClock(); }

void EngineLoop::Run() {
  StartUp();
  while (isGameRunning) {
    Update();
  }
  ShutDown();
}

Clock& EngineLoop::GetGameClock() {
  static Clock gameTime{};
  return gameTime;
}

void RunYidiTest() {
  Input::RegisterKeyPressCallback(KeyCode::T, []() { MemoryManager::Test(); });
}

}  // namespace Isetta
