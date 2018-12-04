/*
 * Copyright (c) 2018 Isetta
 */
#include "Input/InputModule.h"

#include "Core/Debug/Logger.h"
#include "Core/Math/Vector2.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "Input/GLFWInput.h"
#include "Input/Input.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {
// using CBMap = std::unordered_map<int, std::list<std::pair<U16, Action<>>>>;
Delegate<> InputModule::windowCloseCallbacks{};
InputModule::KeyMap InputModule::keyPressDelegates{};
InputModule::KeyMap InputModule::keyReleaseDelegates{};
InputModule::MouseMap InputModule::mousePressDelegates{};
InputModule::MouseMap InputModule::mouseReleaseDelegates{};
Delegate<int, int> InputModule::windowSizeCallbacks;
Delegate<double, double> InputModule::scrollCallbacks;
Delegate<int, int> InputModule::gamepadConnectionCallbacks;

// GLFW
Delegate<GLFWwindow*, int, int, int> InputModule::mouseButtonGLFWCallbacks;
Delegate<GLFWwindow*, int, int, int, int> InputModule::keyGLFWCallbacks;
Delegate<GLFWwindow*, double, double> InputModule::scrollGLFWCallbacks;
Delegate<GLFWwindow*, unsigned int> InputModule::charGLFWCallbacks;
Delegate<GLFWwindow*> InputModule::windowCloseGLFWCallbacks;
Delegate<GLFWwindow*, int, int> InputModule::windowSizeGLFWCallbacks;

GLFWwindow* InputModule::winHandle{nullptr};

void InputModule::RegisterWindowCloseCallback(const Action<>& callback) {
  windowCloseCallbacks.Subscribe(callback);
}
U64 InputModule::RegisterWindowSizeCallback(const Action<int, int>& callback) {
  return windowSizeCallbacks.Subscribe(callback);
}
void InputModule::UnegisterWindowSizeCallback(U64& handle) {
  windowSizeCallbacks.Unsubscribe(handle);
}
bool InputModule::IsKeyPressed(KeyCode key) const {
  int glfwKey = KeyCodeToGlfwKey(key);
  return glfwGetKey(winHandle, glfwKey) == GLFW_PRESS;
}

U64 InputModule::RegisterKeyPressCallback(KeyCode key, ModifierKeys mods,
                                          const Action<>& callback) {
  return RegisterCallback(KeyCodeToGlfwKey(key), mods, callback,
                          &keyPressDelegates);
}
void InputModule::UnregisterKeyPressCallback(KeyCode key, ModifierKeys mods,
                                             U64& handle) {
  UnregisterCallback(KeyCodeToGlfwKey(key), mods, handle, &keyPressDelegates);
}

U64 InputModule::RegisterKeyReleaseCallback(KeyCode key, ModifierKeys mods,
                                            const Action<>& callback) {
  return RegisterCallback(KeyCodeToGlfwKey(key), mods, callback,
                          &keyReleaseDelegates);
}
void InputModule::UnregisterKeyReleaseCallback(KeyCode key, ModifierKeys mods,
                                               U64& handle) {
  UnregisterCallback(KeyCodeToGlfwKey(key), mods, handle, &keyReleaseDelegates);
}

Math::Vector2 InputModule::GetMousePosition() const {
  double xPos, yPos;
  glfwGetCursorPos(winHandle, &xPos, &yPos);
  return Math::Vector2{static_cast<float>(xPos), static_cast<float>(yPos)};
}

bool InputModule::IsMouseButtonPressed(MouseButton mouseButton) const {
  int state = glfwGetMouseButton(winHandle, MouseButtonToGlfwKey(mouseButton));
  return state == GLFW_PRESS;
}

U64 InputModule::RegisterMousePressCallback(MouseButton mouseButton,
                                            const Action<>& callback) {
  return RegisterCallback(MouseButtonToGlfwKey(mouseButton), callback,
                          &mousePressDelegates);
}

void InputModule::UnregisterMousePressCallback(MouseButton mouseButton,
                                               U64& handle) {
  UnregisterCallback(MouseButtonToGlfwKey(mouseButton), handle,
                     &mousePressDelegates);
}

U64 InputModule::RegisterMouseReleaseCallback(MouseButton mouseButton,
                                              const Action<>& callback) {
  return RegisterCallback(MouseButtonToGlfwKey(mouseButton), callback,
                          &mouseReleaseDelegates);
}

void InputModule::UnregisterMouseReleaseCallback(MouseButton mouseButton,
                                                 U64& handle) {
  UnregisterCallback(MouseButtonToGlfwKey(mouseButton), handle,
                     &mouseReleaseDelegates);
}

U64 InputModule::RegisterScrollCallback(
    const Action<double, double>& callback) {
  return scrollCallbacks.Subscribe(callback);
}

void InputModule::UnregisterScrollCallback(U64& handle) {
  scrollCallbacks.Unsubscribe(handle);
}

void InputModule::RegisterWindowCloseGLFWCallback(
    const Action<GLFWwindow*>& callback) {
  windowCloseGLFWCallbacks.Subscribe(callback);
}
U64 InputModule::RegisterWindowSizeGLFWCallback(
    const Action<GLFWwindow*, int, int>& callback) {
  return windowSizeGLFWCallbacks.Subscribe(callback);
}
void InputModule::UnegisterWindowSizeGLFWCallback(U64& handle) {
  windowSizeCallbacks.Unsubscribe(handle);
}

U64 InputModule::RegisterMouseButtonGLFWCallback(
    const Action<GLFWwindow*, int, int, int>& callback) {
  return mouseButtonGLFWCallbacks.Subscribe(callback);
}

void InputModule::UnregisterMouseButtonGLFWCallback(U64& handle) {
  mouseButtonGLFWCallbacks.Unsubscribe(handle);
}

U64 InputModule::RegisterKeyGLFWCallback(
    const Action<GLFWwindow*, int, int, int, int>& callback) {
  return keyGLFWCallbacks.Subscribe(callback);
}

void InputModule::UnegisterKeyGLFWCallback(U64& handle) {
  keyGLFWCallbacks.Unsubscribe(handle);
}

U64 InputModule::RegisterScrollGLFWCallback(
    const Action<GLFWwindow*, double, double>& callback) {
  return scrollGLFWCallbacks.Subscribe(callback);
}

void InputModule::UnregisterScrollGLFWCallback(U64& handle) {
  scrollGLFWCallbacks.Unsubscribe(handle);
}

U64 InputModule::RegisterCharGLFWCallback(
    const Action<GLFWwindow*, unsigned int>& callback) {
  return charGLFWCallbacks.Subscribe(callback);
}

void InputModule::UnegisterCharGLFWCallback(U64& handle) {
  charGLFWCallbacks.Unsubscribe(handle);
}

float InputModule::GetGamepadAxis(GamepadAxis axis) {
  return gamepadState.axes[static_cast<int>(axis)];
}

void InputModule::StartUp(GLFWwindow* win) {
  winHandle = win;
  Input::inputModule = this;
  GLFWInput::inputModule = this;
  glfwSetInputMode(winHandle, GLFW_STICKY_KEYS, 1);
  glfwSetInputMode(winHandle, GLFW_STICKY_MOUSE_BUTTONS, 1);
  glfwSetWindowCloseCallback(winHandle, WindowCloseListener);
  glfwSetWindowSizeCallback(winHandle, WindowSizeListener);
  glfwSetKeyCallback(winHandle, KeyEventListener);
  glfwSetMouseButtonCallback(winHandle, MouseEventListener);
  glfwSetCharCallback(winHandle, CharEventListener);
  glfwSetScrollCallback(winHandle, ScrollEventListener);
  glfwSetJoystickCallback(GamepadEventListener);
}

void InputModule::Update(float deltaTime) {
  BROFILER_CATEGORY("Input Update", Profiler::Color::FireBrick);

  glfwPollEvents();
  UpdateGamepadState();
}

void InputModule::ShutDown() {}

U64 InputModule::RegisterCallback(int key, ModifierKeys mods,
                                  const Action<>& callback,
                                  KeyMap* callbackMap) {
  auto& callbackList = (*callbackMap)[std::make_pair(key, mods)];
  U64 handle = callbackList.Subscribe(callback);
  return handle;
}
void InputModule::UnregisterCallback(int key, ModifierKeys mods, U64& handle,
                                     KeyMap* callbackMap) {
  auto& callbackList = (*callbackMap)[std::make_pair(key, mods)];
  callbackList.Unsubscribe(handle);
}

U64 InputModule::RegisterCallback(int key, const Action<>& callback,
                                  MouseMap* callbackMap) {
  auto& callbackList = (*callbackMap)[key];
  U64 handle = callbackList.Subscribe(callback);
  return handle;
}
void InputModule::UnregisterCallback(int key, U64& handle,
                                     MouseMap* callbackMap) {
  auto& callbackList = (*callbackMap)[key];
  callbackList.Unsubscribe(handle);
}

void InputModule::WindowCloseListener(GLFWwindow* win) {
  windowCloseCallbacks.Invoke();
  windowCloseGLFWCallbacks.Invoke(win);
}

void InputModule::KeyEventListener(GLFWwindow* win, int key, int scancode,
                                   int action, int mods) {
  if (action == GLFW_PRESS) {
    keyPressDelegates[{key, static_cast<ModifierKeys>(mods)}].Invoke();
  } else if (action == GLFW_RELEASE) {
    keyReleaseDelegates[{key, static_cast<ModifierKeys>(mods)}].Invoke();
  }
  keyGLFWCallbacks.Invoke(win, key, scancode, action, mods);
}

void InputModule::MouseEventListener(GLFWwindow* win, int button, int action,
                                     int mods) {
  if (action == GLFW_PRESS) {
    mousePressDelegates[button].Invoke();
  } else if (action == GLFW_RELEASE) {
    mouseReleaseDelegates[button].Invoke();
  }
  mouseButtonGLFWCallbacks.Invoke(win, button, action, mods);
}

void InputModule::CharEventListener(GLFWwindow* win, unsigned int c) {
  charGLFWCallbacks.Invoke(win, c);
}
void InputModule::ScrollEventListener(GLFWwindow* win, double xoffset,
                                      double yoffset) {
  scrollCallbacks.Invoke(xoffset, yoffset);
  scrollGLFWCallbacks.Invoke(win, xoffset, yoffset);
}

void InputModule::WindowSizeListener(GLFWwindow* win, int width, int height) {
  windowSizeCallbacks.Invoke(width, height);
  windowSizeGLFWCallbacks.Invoke(win, width, height);
}

void InputModule::GamepadEventListener(int gamepadID, int gamepadEvent) {
  gamepadConnectionCallbacks.Invoke(gamepadID, gamepadEvent);
}

int InputModule::KeyCodeToGlfwKey(KeyCode key) const {
  int glfwKey;
  switch (key) {
    case KeyCode::NONE:
      glfwKey = GLFW_KEY_UNKNOWN;
      break;
    case KeyCode::SPACE:
      glfwKey = GLFW_KEY_SPACE;
      break;
    case KeyCode::APOSTROPHE:
      glfwKey = GLFW_KEY_APOSTROPHE;
      break;
    case KeyCode::COMMA:
    case KeyCode::MINUS:
    case KeyCode::PERIOD:
    case KeyCode::SLASH:
    case KeyCode::NUM0:
    case KeyCode::NUM1:
    case KeyCode::NUM2:
    case KeyCode::NUM3:
    case KeyCode::NUM4:
    case KeyCode::NUM5:
    case KeyCode::NUM6:
    case KeyCode::NUM7:
    case KeyCode::NUM8:
    case KeyCode::NUM9:
    case KeyCode::SEMICOLON:
    case KeyCode::EQUAL:
      glfwKey = GLFW_KEY_COMMA - static_cast<int>(KeyCode::COMMA) +
                static_cast<int>(key);
      break;
    case KeyCode::A:
    case KeyCode::B:
    case KeyCode::C:
    case KeyCode::D:
    case KeyCode::E:
    case KeyCode::F:
    case KeyCode::G:
    case KeyCode::H:
    case KeyCode::I:
    case KeyCode::J:
    case KeyCode::K:
    case KeyCode::L:
    case KeyCode::M:
    case KeyCode::N:
    case KeyCode::O:
    case KeyCode::P:
    case KeyCode::Q:
    case KeyCode::R:
    case KeyCode::S:
    case KeyCode::T:
    case KeyCode::U:
    case KeyCode::V:
    case KeyCode::W:
    case KeyCode::X:
    case KeyCode::Y:
    case KeyCode::Z:
    case KeyCode::LEFT_BRACKET:
    case KeyCode::BACKSLASH:
    case KeyCode::RIGHT_BRACKET:
      glfwKey =
          GLFW_KEY_A - static_cast<int>(KeyCode::A) + static_cast<int>(key);
      break;
    case KeyCode::GRAVE_ACCENT:
      glfwKey = 96;
      break;
    case KeyCode::ESCAPE:
    case KeyCode::ENTER:
    case KeyCode::TAB:
    case KeyCode::BACKSPACE:
    case KeyCode::INSERT:
    case KeyCode::DEL:
    case KeyCode::RIGHT_ARROW:
    case KeyCode::LEFT_ARROW:
    case KeyCode::DOWN_ARROW:
    case KeyCode::UP_ARROW:
    case KeyCode::PAGE_UP:
    case KeyCode::PAGE_DOWN:
    case KeyCode::HOME:
    case KeyCode::END:
      glfwKey = GLFW_KEY_ESCAPE - static_cast<int>(KeyCode::ESCAPE) +
                static_cast<int>(key);
      break;
    case KeyCode::CAPS_LOCK:
    case KeyCode::SCROLL_LOCK:
    case KeyCode::NUM_LOCK:
    case KeyCode::PRINT_SCREEN:
    case KeyCode::PAUSE:
      glfwKey = GLFW_KEY_CAPS_LOCK - static_cast<int>(KeyCode::CAPS_LOCK) +
                static_cast<int>(key);
      break;
    case KeyCode::F1:
    case KeyCode::F2:
    case KeyCode::F3:
    case KeyCode::F4:
    case KeyCode::F5:
    case KeyCode::F6:
    case KeyCode::F7:
    case KeyCode::F8:
    case KeyCode::F9:
    case KeyCode::F10:
    case KeyCode::F11:
    case KeyCode::F12:
    case KeyCode::F13:
    case KeyCode::F14:
    case KeyCode::F15:
    case KeyCode::F16:
    case KeyCode::F17:
    case KeyCode::F18:
    case KeyCode::F19:
    case KeyCode::F20:
    case KeyCode::F21:
    case KeyCode::F22:
    case KeyCode::F23:
    case KeyCode::F24:
    case KeyCode::F25:
      glfwKey =
          GLFW_KEY_F1 - static_cast<int>(KeyCode::F1) + static_cast<int>(key);
      break;
    case KeyCode::KP_0:
    case KeyCode::KP_1:
    case KeyCode::KP_2:
    case KeyCode::KP_3:
    case KeyCode::KP_4:
    case KeyCode::KP_5:
    case KeyCode::KP_6:
    case KeyCode::KP_7:
    case KeyCode::KP_8:
    case KeyCode::KP_9:
    case KeyCode::KP_DECIMAL:
    case KeyCode::KP_DIVIDE:
    case KeyCode::KP_MULTIPLY:
    case KeyCode::KP_SUBTRACT:
    case KeyCode::KP_ADD:
    case KeyCode::KP_ENTER:
    case KeyCode::KP_EQUAL:
      glfwKey = GLFW_KEY_KP_0 - static_cast<int>(KeyCode::KP_0) +
                static_cast<int>(key);
      break;
    case KeyCode::LEFT_SHIFT:
    case KeyCode::LEFT_CONTROL:
    case KeyCode::LEFT_ALT:
    case KeyCode::LEFT_SUPER:
    case KeyCode::RIGHT_SHIFT:
    case KeyCode::RIGHT_CONTROL:
    case KeyCode::RIGHT_ALT:
    case KeyCode::RIGHT_SUPER:
    case KeyCode::MENU:
      glfwKey = GLFW_KEY_LEFT_SHIFT - static_cast<int>(KeyCode::LEFT_SHIFT) +
                static_cast<int>(key);
      break;
    default:
      glfwKey = GLFW_KEY_UNKNOWN;
      break;
  }
  return glfwKey;
}

int InputModule::MouseButtonToGlfwKey(MouseButton mouseButton) const {
  return static_cast<int>(mouseButton);
}

void InputModule::UpdateGamepadState() {
  glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState);
  DeadZoneOptimize(&gamepadState.axes[0], &gamepadState.axes[1]);
  DeadZoneOptimize(&gamepadState.axes[2], &gamepadState.axes[3]);
}

void InputModule::DeadZoneOptimize(float* horizontal, float* verticle) {
  Math::Vector2 axis{*horizontal, *verticle};

  if (axis.Magnitude() < 0.15f) {
    axis = Math::Vector2::zero;
  } else {
    axis = axis.Normalized() * ((axis.Magnitude() - 0.15f) / (1 - 0.15f));
  }
  *horizontal = axis.x;
  *verticle = axis.y;
}

bool InputModule::IsGamepadButtonPressed(GamepadButton button) {
  return gamepadState.buttons[static_cast<int>(button)];
}

U64 InputModule::RegisterGamepadConnectionCallback(
    const Action<int, int>& callback) {
  return gamepadConnectionCallbacks.Subscribe(callback);
}

void InputModule::UnegisterGamepadConnectionCallback(U64& handle) {
  gamepadConnectionCallbacks.Unsubscribe(handle);
}
void InputModule::Clear() {
  windowCloseCallbacks.Clear();
  keyPressDelegates.clear();
  keyReleaseDelegates.clear();
  mousePressDelegates.clear();
  mouseReleaseDelegates.clear();
  windowSizeCallbacks.Clear();
  scrollCallbacks.Clear();
  gamepadConnectionCallbacks.Clear();
}
}  // namespace Isetta
