/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/IsettaAlias.h"
#include "Core/Math/Vector2.h"
#include "Input/KeyCode.h"

namespace Isetta {
class ISETTA_API_DECLARE Input {
 public:
  /**
   * \brief Register a callback function to window close event
   * \param callback The callback function
   */
  static void RegisterWindowCloseCallback(const Action<>& callback);
  static U16 RegisterWindowSizeCallback(const Action<int, int>& callback);
  static void UnegisterWindowSizeCallback(U64& handle);
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
  static U64 RegisterKeyPressCallback(KeyCode key, ModifierKeys mods,
                                      const Action<>& callback);
  /**
   * \brief Unregister a callback by the key and handle
   * \param key The key to detect
   * \param handle The handle to unregister
   */
  static void UnregisterKeyPressCallback(KeyCode key, U64& handle);
  static void UnregisterKeyPressCallback(KeyCode key, ModifierKeys mods,
                                         U64& handle);
  /**
   * \brief Register a callback function to the key release event and return its
   * handle
   * \param key The keycode to detect
   * \param callback The callback function
   */
  static U64 RegisterKeyReleaseCallback(KeyCode key, const Action<>& callback);
  static U64 RegisterKeyReleaseCallback(KeyCode key, ModifierKeys mods,
                                        const Action<>& callback);
  /**
   * \brief Unregister a callback by the key and handle
   * \param key The key to detect
   * \param handle The handle to unregister
   */
  static void UnregisterKeyReleaseCallback(KeyCode key, U64& handle);
  static void UnregisterKeyReleaseCallback(KeyCode key, ModifierKeys mods,
                                           U64& handle);
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
                                           U64& handle);
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
                                             U64& handle);

  static U16 RegisterScrollCallback(const Action<double, double>& callback);
  static void UnregisterScrollCallback(U64& handle);

  static float GetGamepadAxis(GamepadAxis axis);
  static bool IsGamepadButtonPressed(GamepadButton button);

  static void Clear();

 private:
  static class InputModule* inputModule;

  friend class InputModule;
};
}  // namespace Isetta
