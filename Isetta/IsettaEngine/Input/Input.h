/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector2.h"
#include "Input/InputEnum.h"

namespace Isetta {
class ISETTA_API_DECLARE Input {
 public:
  /**
   * \brief Register a callback function to window close event
   * \param callback The callback function
   */
  static void RegisterWindowCloseCallback(const Action<>& callback);
  static U64 RegisterWinSizeCallback(const Action<int, int>& callback);
  static void UnegisterWindowSizeCallback(U64 handle);
  /**
   * \brief Check if the key is pressed
   * \param key The keycode to detect
   */
  static bool IsKeyPressed(KeyCode key);
  /**
   * \brief Register a callback function to the key press event and return its
   * handle
   * \param key The keycode to detect
   * \param callback The callback function
   */
  static U64 RegisterKeyPressCallback(KeyCode key, const Action<>& callback);
  /**
   * \brief Unregister a callback by the key and handle
   * \param key The key to detect
   * \param handle The handle to unregister
   */
  static void UnregisterKeyPressCallback(KeyCode key, U64 handle);
  /**
   * \brief Register a callback function to the key release event and return its
   * handle
   * \param key The keycode to detect
   * \param callback The callback function
   */
  static U64 RegisterKeyReleaseCallback(KeyCode key, const Action<>& callback);
  /**
   * \brief Unregister a callback by the key and handle
   * \param key The key to detect
   * \param handle The handle to unregister
   */
  static void UnregisterKeyReleaseCallback(KeyCode key, U64 handle);
  /**
   * \brief Get the position of the mouse
   */
  static Math::Vector2 GetMousePosition();
  /**
   * \brief Check if the mouse button is pressed
   * \param mouseButton The mouse button to detect
   */
  static bool IsMouseButtonPressed(MouseButtonCode mouseButton);
  /**
   * \brief Register a callback function to the mouse press event and return its
   * handle
   * \param mouseButton The mouse button to detect
   * \param callback The callback function
   */
  static U64 RegisterMousePressCallback(MouseButtonCode mouseButton,
                                        const Action<>& callback);
  /**
   * \brief Unregister a callback by the mouse button and handle
   * \param mouseButton The mouse button to detect
   * \param handle The handle to unregister
   */
  static void UnregisterMousePressCallback(MouseButtonCode mouseButton,
                                           U64 handle);
  /**
   * \brief Register a callback function to the mouse release event and return
   * its handle
   * \param mouseButton The mouse button to detect
   * \param callback The callback function
   */
  static U64 RegisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                          const Action<>& callback);
  /**
   * \brief Unregister a callback by the mouse button and handle
   * \param mouseButton The mouse button to detect
   * \param handle The handle to unregister
   */
  static void UnregisterMouseReleaseCallback(MouseButtonCode mouseButton,
                                             U64 handle);

  static U64 RegisterMouseButtonCallback(
      const Action<GLFWwindow*, int, int, int>& callback);
  static void UnregisterMouseButtonCallback(U64 handle);
  static U64 RegisterKeyCallback(
      const Action<GLFWwindow*, int, int, int, int>& callback);
  static void UnegisterKeyCallback(U64 handle);
  static U64 RegisterScrollCallback(
      const Action<GLFWwindow*, double, double>& callback);
  static void UnegisterScrollCallback(U64 handle);
  static U64 RegisterCharCallback(
      const Action<GLFWwindow*, unsigned int>& callback);
  static void UnegisterCharCallback(U64 handle);

 private:
  static class InputModule* inputModule;

  friend class InputModule;
};
}  // namespace Isetta
