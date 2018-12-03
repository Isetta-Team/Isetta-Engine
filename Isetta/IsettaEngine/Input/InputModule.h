/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <unordered_map>
#include <utility>
#include "Core/DataStructures/Delegate.h"
#include "Core/IsettaAlias.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "Input/KeyCode.h"
#include "Util.h"

namespace Isetta::Math {
class Vector2;
}

namespace Isetta {
class InputModule {
  // using CBMap = std::unordered_map<int, std::list<std::pair<U16, Action<>>>>;
  using KeyMap = std::unordered_map<std::pair<int, ModifierKeys>, Delegate<>,
                                    Util::PairHash, Util::PairHash>;
  using MouseMap = std::unordered_map<int, Delegate<>>;

 public:
  /**
   * \brief Register a callback function to window close event
   * \param callback The callback function
   */
  void RegisterWindowCloseCallback(const Action<>& callback);
  U64 RegisterWindowSizeCallback(const Action<int, int>& callback);
  void UnegisterWindowSizeCallback(U64& handle);
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
  U64 RegisterKeyPressCallback(KeyCode key, ModifierKeys mods,
                               const Action<>& callback);
  /**
   * \brief Unregister a callback by the key and handle
   * \param key The key to detect
   * \param handle The handle to unregister
   */
  void UnregisterKeyPressCallback(KeyCode key, ModifierKeys mods, U64& handle);
  /**
   * \brief Register a callback function to the key release event and return its
   * handle
   * \param key The keycode to detect
   * \param callback The callback function
   */
  U64 RegisterKeyReleaseCallback(KeyCode key, ModifierKeys mods,
                                 const Action<>& callback);
  /**
   * \brief Unregister a callback by the key and handle
   * \param key The key to detect
   * \param handle The handle to unregister
   */
  void UnregisterKeyReleaseCallback(KeyCode key, ModifierKeys mods, U64& handle);
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
  U64 RegisterMousePressCallback(MouseButtonCode mouseButton,
                                 const Action<>& callback);
  /**
   * \brief Unregister a callback by the mouse button and handle
   * \param mouseButton The mouse button to detect
   * \param handle The handle to unregister
   */
  void UnregisterMousePressCallback(MouseButtonCode mouseButton, U64& handle);
  /**
   * \brief Register a callback function to the mouse release event and return
   * its handle
   * \param mouseButton The mouse button to detect
   * \param callback The callback function
   */
  U64 RegisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                   const Action<>& callback);
  /**
   * \brief Unregister a callback by the mouse button and handle
   * \param mouseButton The mouse button to detect
   * \param handle The handle to unregister
   */
  void UnregisterMouseReleaseCallback(MouseButtonCode mouseButton, U64& handle);

  U64 RegisterScrollCallback(const Action<double, double>& callback);
  void UnregisterScrollCallback(U64& handle);

  float GetGamepadAxis(GamepadAxis axis);
  bool IsGamepadButtonPressed(GamepadButton button);
  U64 RegisterGamepadConnectionCallback(const Action<int, int>& callback);
  void UnegisterGamepadConnectionCallback(U64& handle);

  void Clear();

  // TODO(Chaojie) + TODO(Jacob): Talk about these, should unregister return
  // bool?
  void RegisterWindowCloseGLFWCallback(const Action<GLFWwindow*>& callback);
  U64 RegisterWindowSizeGLFWCallback(
      const Action<GLFWwindow*, int, int>& callback);
  void UnegisterWindowSizeGLFWCallback(U64& handle);
  U64 RegisterMouseButtonGLFWCallback(
      const Action<GLFWwindow*, int, int, int>& callback);
  void UnregisterMouseButtonGLFWCallback(U64& handle);
  U64 RegisterKeyGLFWCallback(
      const Action<GLFWwindow*, int, int, int, int>& callback);
  void UnegisterKeyGLFWCallback(U64& handle);
  U64 RegisterScrollGLFWCallback(
      const Action<GLFWwindow*, double, double>& callback);
  void UnregisterScrollGLFWCallback(U64& handle);
  U64 RegisterCharGLFWCallback(
      const Action<GLFWwindow*, unsigned int>& callback);
  void UnegisterCharGLFWCallback(U64& handle);

 private:
  static GLFWwindow* winHandle;

  InputModule() = default;
  ~InputModule() = default;

  void StartUp(GLFWwindow* win);
  void Update(float deltaTime);
  void ShutDown();

  U64 RegisterCallback(int key, ModifierKeys mods, const Action<>& callback,
                       KeyMap* delegateMap);
  void UnregisterCallback(int key, ModifierKeys mods, U64& handle,
                          KeyMap* delegateMap);
  U64 RegisterCallback(int key, const Action<>& callback,
                       MouseMap* delegateMap);
  void UnregisterCallback(int key, U64& handle, MouseMap* delegateMap);
  int KeyCodeToGlfwKey(KeyCode key) const;
  int MouseButtonToGlfwKey(MouseButtonCode mouseButton) const;
  GLFWgamepadstate gamepadState;
  void UpdateGamepadState();
  void DeadZoneOptimize(float* horizontal, float* verticle);

  static Delegate<> windowCloseCallbacks;
  static KeyMap keyPressDelegates;
  static KeyMap keyReleaseDelegates;
  static MouseMap mousePressDelegates;
  static MouseMap mouseReleaseDelegates;
  static Delegate<int, int> windowSizeCallbacks;
  static Delegate<double, double> scrollCallbacks;
  static Delegate<int, int> gamepadConnectionCallbacks;

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
  static Delegate<GLFWwindow*> windowCloseGLFWCallbacks;
  static Delegate<GLFWwindow*, int, int> windowSizeGLFWCallbacks;
  static Delegate<GLFWwindow*, int, int, int> mouseButtonGLFWCallbacks;
  static Delegate<GLFWwindow*, int, int, int, int> keyGLFWCallbacks;
  static Delegate<GLFWwindow*, double, double> scrollGLFWCallbacks;
  static Delegate<GLFWwindow*, unsigned int> charGLFWCallbacks;

  friend class EngineLoop;
  friend class StackAllocator;
};
}  // namespace Isetta
