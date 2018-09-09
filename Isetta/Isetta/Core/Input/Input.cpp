/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Input/Input.h"
#include <GLFW/glfw3.h>
#include "Core/Debug/Logger.h"
#include "Core/Graphics/Render.h"
#include "Core/Input/InputInterface.h"

namespace Isetta {
std::list<std::function<void()>> InputModule::windowCloseCallbacks{};
std::unordered_map<int, std::list<std::function<void()>>>
    InputModule::keyPressCallbacks{};
std::unordered_map<int, std::list<std::function<void()>>>
    InputModule::keyReleaseCallbacks{};
GLFWwindow* InputModule::winHandle{nullptr};

InputModule::InputModule(GLFWwindow* win) { winHandle = win; }

void InputModule::RegisterWindowCloseCallback(std::function<void()> callback) {
  windowCloseCallbacks.push_back(callback);
}
bool InputModule::IsKeyPressed(KeyCode key) {
  int glfw_key = KeyCodeToGLFWKey(key);
  return glfwGetKey(winHandle, glfw_key) == GLFW_PRESS;
}

void InputModule::RegisterKeyPressCallback(
    KeyCode key, const std::function<void()>& callback) {
  int glfwKey = KeyCodeToGLFWKey(key);
  keyPressCallbacks[glfwKey].push_back(callback);
}

void InputModule::RegisterKeyReleaseCallback(
    KeyCode key, const std::function<void()>& callback) {
  int glfwKey = KeyCodeToGLFWKey(key);
  keyReleaseCallbacks[glfwKey].push_back(callback);
}

void InputModule::StartUp() {
  Input::inputModule = this;
  glfwSetWindowCloseCallback(winHandle, windowCloseListener);
  glfwSetKeyCallback(winHandle, keyEventListener);
}

void InputModule::Update() { glfwPollEvents(); }

void InputModule::ShutDown() {}

int InputModule::KeyCodeToGLFWKey(KeyCode key) {
  int glfw_key{-1};
  switch (key) {
    case KeyCode::NONE:
      glfw_key = GLFW_KEY_UNKNOWN;
      break;
    case KeyCode::SPACE:
      glfw_key = GLFW_KEY_SPACE;
      break;
    case KeyCode::APOSTROPHE:
      glfw_key = GLFW_KEY_APOSTROPHE;
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
      glfw_key = GLFW_KEY_A - (int)KeyCode::A + (int)key;
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
      glfw_key = GLFW_KEY_A - (int)KeyCode::A + (int)key;
      break;
    case KeyCode::GRAVE_ACCENT:
      glfw_key = 96;
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
      glfw_key = GLFW_KEY_ESCAPE - (int)KeyCode::ESCAPE + (int)key;
      break;
    default:
      glfw_key = GLFW_KEY_UNKNOWN;
      break;
  }
  return glfw_key;
}

void InputModule::windowCloseListener(GLFWwindow* win) {
  for (const auto& callback : windowCloseCallbacks) {
    callback();
  }
}
void InputModule::keyEventListener(GLFWwindow* win, int key, int scancode,
                                   int action, int mods) {
  if (action == GLFW_PRESS) {
    for (const auto& callback : keyPressCallbacks[key]) {
      callback();
    }
  } else if (action == GLFW_RELEASE) {
    for (const auto& callback : keyReleaseCallbacks[key]) {
      callback();
    }
  }
}
}  // namespace Isetta