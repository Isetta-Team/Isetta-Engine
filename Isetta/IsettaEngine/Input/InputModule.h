/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include <list>
#include <unordered_map>
#include <utility>
#include "Core/IsettaAlias.h"
#include "Input/KeyCode.h"
#include "Util.h"

namespace Isetta::Math {
class Vector2;
}

namespace Isetta {
class InputModule {
  // using CBMap = std::unordered_map<int, std::list<std::pair<U16, Action<>>>>;
  using KeyMap =
      std::unordered_map<std::pair<int, ModifierKeys>,
                         std::list<std::pair<U16, Action<>>>, Util::PairHash>;
  using MouseMap = std::unordered_map<int, std::list<std::pair<U16, Action<>>>>;

 public:
  /**
   * \brief Register a callback function to window close event
   * \param callback The callback function
   */
  void RegisterWindowCloseCallback(const Action<>& callback);
  U16 RegisterWindowSizeCallback(const Action<int, int>& callback);
  void UnegisterWindowSizeCallback(U16 handle);
  /**
   * \brief Check if the key is pressed
   * \param key The keycode to detect
   */
  bool IsKeyPressed(KeyCode key) const;
  /**
   * \brief Register a callback function to the key press event and return its
   * handle
   * \param key The keycode to detect
   * \param callback The callback function
   */
  U16 RegisterKeyPressCallback(KeyCode key, ModifierKeys mods,
                               const Action<>& callback);
  /**
   * \brief Unregister a callback by the key and handle
   * \param key The key to detect
   * \param handle The handle to unregister
   */
  void UnregisterKeyPressCallback(KeyCode key, ModifierKeys mods, U16 handle);
  /**
   * \brief Register a callback function to the key release event and return its
   * handle
   * \param key The keycode to detect
   * \param callback The callback function
   */
  U16 RegisterKeyReleaseCallback(KeyCode key, ModifierKeys mods,
                                 const Action<>& callback);
  /**
   * \brief Unregister a callback by the key and handle
   * \param key The key to detect
   * \param handle The handle to unregister
   */
  void UnregisterKeyReleaseCallback(KeyCode key, ModifierKeys mods, U16 handle);
  /**
   * \brief Get the position of the mouse
   */
  Math::Vector2 GetMousePosition() const;
  /**
   * \brief Check if the mouse button is pressed
   * \param mouseButton The mouse button to detect
   */
  bool IsMouseButtonPressed(MouseButtonCode mouseButton) const;
  /**
   * \brief Register a callback function to the mouse press event and return its
   * handle
   * \param mouseButton The mouse button to detect
   * \param callback The callback function
   */
  U16 RegisterMousePressCallback(MouseButtonCode mouseButton,
                                 const Action<>& callback);
  /**
   * \brief Unregister a callback by the mouse button and handle
   * \param mouseButton The mouse button to detect
   * \param handle The handle to unregister
   */
  void UnregisterMousePressCallback(MouseButtonCode mouseButton, U16 handle);
  /**
   * \brief Register a callback function to the mouse release event and return
   * its handle
   * \param mouseButton The mouse button to detect
   * \param callback The callback function
   */
  U16 RegisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                   const Action<>& callback);
  /**
   * \brief Unregister a callback by the mouse button and handle
   * \param mouseButton The mouse button to detect
   * \param handle The handle to unregister
   */
  void UnregisterMouseReleaseCallback(MouseButtonCode mouseButton, U16 handle);

  U16 RegisterScrollCallback(const Action<double, double>& callback);
  void UnregisterScrollCallback(U16 handle);

  float GetGamepadAxis(GamepadAxis axis);
  bool IsGamepadButtonPressed(GamepadButton button);
  U16 RegisterGamepadConnectionCallback(const Action<int, int>& callback);
  void UnegisterGamepadConnectionCallback(U16 handle);

  void Clear();

  // TODO(Chaojie) + TODO(Jacob): Talk about these, should unregister return
  // bool?
  void RegisterWindowCloseGLFWCallback(const Action<GLFWwindow*>& callback);
  U16 RegisterWindowSizeGLFWCallback(
      const Action<GLFWwindow*, int, int>& callback);
  void UnegisterWindowSizeGLFWCallback(U16 handle);
  U16 RegisterMouseButtonGLFWCallback(
      const Action<GLFWwindow*, int, int, int>& callback);
  void UnregisterMouseButtonGLFWCallback(U16 handle);
  U16 RegisterKeyGLFWCallback(
      const Action<GLFWwindow*, int, int, int, int>& callback);
  void UnegisterKeyGLFWCallback(U16 handle);
  U16 RegisterScrollGLFWCallback(
      const Action<GLFWwindow*, double, double>& callback);
  void UnregisterScrollGLFWCallback(U16 handle);
  U16 RegisterCharGLFWCallback(
      const Action<GLFWwindow*, unsigned int>& callback);
  void UnegisterCharGLFWCallback(U16 handle);

 private:
  static GLFWwindow* winHandle;

  InputModule() = default;
  ~InputModule() = default;

  void StartUp(GLFWwindow* win);
  void Update(float deltaTime);
  void ShutDown();

  U16 RegisterCallback(int key, ModifierKeys mods, const Action<>& callback,
                       KeyMap* callbackMap);
  void UnregisterCallback(int key, ModifierKeys mods, U16 handle,
                          KeyMap* callbackMap);
  U16 RegisterCallback(int key, const Action<>& callback,
                       MouseMap* callbackMap);
  void UnregisterCallback(int key, U16 handle, MouseMap* callbackMap);
  int KeyCodeToGlfwKey(KeyCode key) const;
  int MouseButtonToGlfwKey(MouseButtonCode mouseButton) const;
  GLFWgamepadstate gamepadState;
  void UpdateGamepadState();
  void DeadZoneOptimize(float* horizontal, float* verticle);

  static std::list<Action<>> windowCloseCallbacks;
  static KeyMap keyPressCallbacks;
  static KeyMap keyReleaseCallbacks;
  static MouseMap mousePressCallbacks;
  static MouseMap mouseReleaseCallbacks;
  static std::unordered_map<U16, Action<int, int>> windowSizeCallbacks;
  static std::unordered_map<U16, Action<double, double>> scrollCallbacks;
  static std::unordered_map<U16, Action<int, int>> gamepadConnectionCallbacks;

  static void WindowCloseListener(GLFWwindow* win);
  static void KeyEventListener(GLFWwindow* win, int key, int scancode,
                               int action, int mods);
  static void MouseEventListener(GLFWwindow* win, int button, int action,
                                 int mods);
  static void CharEventListener(GLFWwindow*, unsigned int c);
  static void ScrollEventListener(GLFWwindow*, double xoffset, double yoffset);
  static void GamepadEventListener(int gamepadID, int gamepadEvent);
  static void WindowSizeListener(GLFWwindow* win, int width, int height);

  // GLFW Callbacks
  static std::list<Action<GLFWwindow*>> windowCloseGLFWCallbacks;
  static std::unordered_map<U16, Action<GLFWwindow*, int, int>>
      windowSizeGLFWCallbacks;
  static std::unordered_map<U16, Action<GLFWwindow*, int, int, int>>
      mouseButtonGLFWCallbacks;
  static std::unordered_map<U16, Action<GLFWwindow*, int, int, int, int>>
      keyGLFWCallbacks;
  static std::unordered_map<U16, Action<GLFWwindow*, double, double>>
      scrollGLFWCallbacks;
  static std::unordered_map<U16, Action<GLFWwindow*, unsigned int>>
      charGLFWCallbacks;

  static U16 inputHandle;
  static U8 glfwHandle;

  friend class EngineLoop;
};
}  // namespace Isetta
