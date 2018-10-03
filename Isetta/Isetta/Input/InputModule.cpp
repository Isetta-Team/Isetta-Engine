/*
 * Copyright (c) 2018 Isetta
 */
#include "Input/InputModule.h"

#include <GLFW/glfw3.h>
#include "Core/Debug/Logger.h"
#include "Core/Math/Vector2.h"
#include "Input/Input.h"

namespace Isetta {
using CBMap = std::unordered_map<int, std::list<std::pair<U64, Action<>>>>;

std::list<Action<>> InputModule::windowCloseCallbacks{};
CBMap InputModule::keyPressCallbacks{};
CBMap InputModule::keyReleaseCallbacks{};
CBMap InputModule::mousePressCallbacks{};
CBMap InputModule::mouseReleaseCallbacks{};

std::unordered_map<U64, Action<GLFWwindow*, int, int, int>>
    InputModule::mouseButtonCallbacks;
std::unordered_map<U64, Action<GLFWwindow*, int, int, int, int>>
    InputModule::keyCallbacks;
std::unordered_map<U64, Action<GLFWwindow*, double, double>>
    InputModule::scrollCallbacks;
std::unordered_map<U64, Action<GLFWwindow*, unsigned int>>
    InputModule::charCallbacks;

U64 InputModule::totalHandle{};

GLFWwindow* InputModule::winHandle{nullptr};

void InputModule::RegisterWindowCloseCallback(const Action<>& callback) {
  windowCloseCallbacks.push_back(callback);
}
bool InputModule::IsKeyPressed(KeyCode key) const {
  int glfwKey = KeyCodeToGlfwKey(key);
  return glfwGetKey(winHandle, glfwKey) == GLFW_PRESS;
}

U64 InputModule::RegisterKeyPressCallback(KeyCode key,
                                          const Action<>& callback) {
  return RegisterCallback(KeyCodeToGlfwKey(key), callback, &keyPressCallbacks);
}

void InputModule::UnregisterKeyPressCallback(KeyCode key, U64 handle) {
  UnregisterCallback(KeyCodeToGlfwKey(key), handle, &keyPressCallbacks);
}

U64 InputModule::RegisterKeyReleaseCallback(KeyCode key,
                                            const Action<>& callback) {
  return RegisterCallback(KeyCodeToGlfwKey(key), callback,
                          &keyReleaseCallbacks);
}

void InputModule::UnregisterKeyReleaseCallback(KeyCode key, U64 handle) {
  UnregisterCallback(KeyCodeToGlfwKey(key), handle, &keyReleaseCallbacks);
}

Math::Vector2 InputModule::GetMousePosition() const {
  double xPos, yPos;
  glfwGetCursorPos(winHandle, &xPos, &yPos);
  return Math::Vector2{static_cast<float>(xPos), static_cast<float>(yPos)};
}

bool InputModule::IsMouseButtonPressed(MouseButtonCode mouseButton) const {
  int state = glfwGetMouseButton(winHandle, MouseButtonToGlfwKey(mouseButton));
  return state == GLFW_PRESS;
}

U64 InputModule::RegisterMousePressCallback(MouseButtonCode mouseButton,
                                            const Action<>& callback) {
  return RegisterCallback(MouseButtonToGlfwKey(mouseButton), callback,
                          &mousePressCallbacks);
}

void InputModule::UnregisterMousePressCallback(MouseButtonCode mouseButton,
                                               U64 handle) {
  UnregisterCallback(MouseButtonToGlfwKey(mouseButton), handle,
                     &mousePressCallbacks);
}

U64 InputModule::RegisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                              const Action<>& callback) {
  return RegisterCallback(MouseButtonToGlfwKey(mouseButton), callback,
                          &mouseReleaseCallbacks);
}

void InputModule::UnregisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                                 U64 handle) {
  UnregisterCallback(MouseButtonToGlfwKey(mouseButton), handle,
                     &mouseReleaseCallbacks);
}

U64 InputModule::RegisterMouseButtonCallback(
    const Action<GLFWwindow*, int, int, int>& callback) {
  U64 handle = totalHandle++;
  mouseButtonCallbacks.insert(std::make_pair(handle, callback));
  return handle;
}

void InputModule::UnregisterMouseButtonCallback(U64 handle) {
  mouseButtonCallbacks.erase(handle);
}

U64 InputModule::RegisterKeyCallback(
    const Action<GLFWwindow*, int, int, int, int>& callback) {
  U64 handle = totalHandle++;
  keyCallbacks.insert(std::make_pair(handle, callback));
  return handle;
}

void InputModule::UnegisterKeyCallback(U64 handle) {
  keyCallbacks.erase(handle);
}

U64 InputModule::RegisterScrollCallback(
    const Action<GLFWwindow*, double, double>& callback) {
  U64 handle = totalHandle++;
  scrollCallbacks.insert(std::make_pair(handle, callback));
  return handle;
}

void InputModule::UnegisterScrollCallback(U64 handle) {
  scrollCallbacks.erase(handle);
}

U64 InputModule::RegisterCharCallback(
    const Action<GLFWwindow*, unsigned int>& callback) {
  U64 handle = totalHandle++;
  charCallbacks.insert(std::make_pair(handle, callback));
  return handle;
}

void InputModule::UnegisterCharCallback(U64 handle) {
  charCallbacks.erase(handle);
}

void InputModule::StartUp(GLFWwindow* win) {
  winHandle = win;
  Input::inputModule = this;
  glfwSetInputMode(winHandle, GLFW_STICKY_KEYS, 1);
  glfwSetInputMode(winHandle, GLFW_STICKY_MOUSE_BUTTONS, 1);
  glfwSetWindowCloseCallback(winHandle, WindowCloseListener);
  glfwSetKeyCallback(winHandle, KeyEventListener);
  glfwSetMouseButtonCallback(winHandle, MouseEventListener);
  glfwSetCharCallback(winHandle, CharEventListener);
  glfwSetScrollCallback(winHandle, ScrollEventListener);
}

void InputModule::Update(float deltaTime) { glfwPollEvents(); }

void InputModule::ShutDown() {}

U64 InputModule::RegisterCallback(int key, const Action<>& callback,
                                  CBMap* callbackMap) {
  auto& callbackList = (*callbackMap)[key];
  U64 handle = totalHandle++;
  callbackList.push_back(std::make_pair(handle, callback));
  return handle;
}

void InputModule::UnregisterCallback(int key, U64 handle, CBMap* callbackMap) {
  auto& callbackList = (*callbackMap)[key];
  callbackList.remove_if(
      [handle](std::pair<U64, Action<>> item) { return item.first == handle; });
}

void InputModule::WindowCloseListener(GLFWwindow* win) {
  for (const auto& callback : windowCloseCallbacks) {
    callback();
  }
}

void InputModule::KeyEventListener(GLFWwindow* win, int key, int scancode,
                                   int action, int mods) {
  // static std::list<Action<)>> currCallbacks;
  if (action == GLFW_PRESS) {
    for (const auto& handleCallback : keyPressCallbacks[key]) {
      // currCallbacks.push_back(callback.second);
      handleCallback.second();
    }
  } else if (action == GLFW_RELEASE) {
    for (const auto& handleCallback : keyReleaseCallbacks[key]) {
      // currCallbacks.push_back(callback.second);
      handleCallback.second();
    }
  }
  // for (const auto& callback : currCallbacks) {
  //  callback();
  //}
  // currCallbacks.clear();
  // TODO(Chaojie + Jacob): can we pass this information to all the functions?
  for (const auto& handleCallback : keyCallbacks) {
    handleCallback.second(win, key, scancode, action, mods);
  }
}

void InputModule::MouseEventListener(GLFWwindow* win, int button, int action,
                                     int mods) {
  static std::list<Action<>> currCallbacks;
  if (action == GLFW_PRESS) {
    for (const auto& handleCallback : mousePressCallbacks[button]) {
      // currCallbacks.push_back(callback.second);
      handleCallback.second();
    }
  } else if (action == GLFW_RELEASE) {
    for (const auto& handleCallback : mouseReleaseCallbacks[button]) {
      // currCallbacks.push_back(callback.second);
      handleCallback.second();
    }
  }
  // for (const auto& callback : currCallbacks) {
  //  callback();
  //}
  // currCallbacks.clear();
  // TODO(Chaojie + Jacob): can we pass this information to all the functions?
  for (const auto& handleCallback : mouseButtonCallbacks) {
    handleCallback.second(win, button, action, mods);
  }
}

void InputModule::CharEventListener(GLFWwindow* win, unsigned int c) {
  for (const auto& handleCallback : charCallbacks) {
    handleCallback.second(win, c);
  }
}
void InputModule::ScrollEventListener(GLFWwindow* win, double xoffset,
                                      double yoffset) {
  for (const auto& handleCallback : scrollCallbacks) {
    handleCallback.second(win, xoffset, yoffset);
  }
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
    case KeyCode::RIGHT:
    case KeyCode::LEFT:
    case KeyCode::DOWN:
    case KeyCode::UP:
    case KeyCode::PAGE_UP:
    case KeyCode::PAGE_DOWN:
    case KeyCode::HOME:
    case KeyCode::END:
    case KeyCode::CAPS_LOCK:
    case KeyCode::SCROLL_LOCK:
    case KeyCode::NUM_LOCK:
    case KeyCode::PRINT_SCREEN:
    case KeyCode::PAUSE:
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
    case KeyCode::LEFT_SHIFT:
    case KeyCode::LEFT_CONTROL:
    case KeyCode::LEFT_ALT:
    case KeyCode::LEFT_SUPER:
    case KeyCode::RIGHT_SHIFT:
    case KeyCode::RIGHT_CONTROL:
    case KeyCode::RIGHT_ALT:
    case KeyCode::RIGHT_SUPER:
    case KeyCode::MENU:
      glfwKey = GLFW_KEY_ESCAPE - static_cast<int>(KeyCode::ESCAPE) +
                static_cast<int>(key);
      break;
    default:
      glfwKey = GLFW_KEY_UNKNOWN;
      break;
  }
  return glfwKey;
}

int InputModule::MouseButtonToGlfwKey(MouseButtonCode mouseButton) const {
  return static_cast<int>(mouseButton);
}
}  // namespace Isetta
