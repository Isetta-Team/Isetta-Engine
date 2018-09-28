/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#ifndef __  // GLAD must be placed first
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <list>
#include <string>
#include <unordered_map>

#include "Core/Config/CVar.h"
#include "Core/IsettaAlias.h"

#include "imgui/imgui.h"

class ImVec4;
namespace Isetta::Math {
class Vector2;
}

namespace Isetta {
class GUIModule {
 public:
  struct GUIModuleConfig {};

  void OnUpdate(const Action<>&);
  inline Math::Vector2 GetWindowSize() const {
    return Math::Vector2(winWidth, winHeight);
  }

 private:
  GUIModule() = default;
  ~GUIModule() = default;

  void StartUp(GLFWwindow* win);
  void Update(float deltaTime);
  void ShutDown();

  // TODO(Jacob)
  // ImGui::SetAllocatorFunctions line 1383

  GLFWwindow* winHandle;
  int winWidth, winHeight;

  std::list<Action<>> updateCallbacks{};

  friend class ModuleManager;

  // TODO(Jacob) remove
  // Test variables
  // bool show_demo_window, show_another_window;
  // ImVec4 clear_color;
};
}  // namespace Isetta
