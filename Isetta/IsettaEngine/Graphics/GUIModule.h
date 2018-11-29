/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#ifndef __  // GLAD must be placed first
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include "Core/Color.h"
#include "Core/Config/CVar.h"
#include "Util.h"

namespace Isetta::Math {
class Vector2Int;
}

namespace Isetta {
class GUIModule {
 public:
  struct GUIModuleConfig {
    CVar<Color> Text{"text_style", Color{}};
    CVar<Color> TextDisabled{"text_disabled_style", Color{}};
    CVar<Color> WindowBg{"window_bg_style_style", Color{}};
    CVar<Color> ChildBg{"child_bg_style", Color{}};
    CVar<Color> PopupBg{"popup_bg_style", Color{}};
    CVar<Color> Border{"border_style", Color{}};
    CVar<Color> BorderShadow{"border_shadow_style", Color{}};
    CVar<Color> FrameBg{"frame_bg_style", Color{}};
    CVar<Color> FrameBgHovered{"frame_bg_hovered_style", Color{}};
    CVar<Color> FrameBgActive{"frame_bg_active_style", Color{}};
    CVar<Color> TitleBg{"title_bg_style", Color{}};
    CVar<Color> TitleBgActive{"title_bg_active_style", Color{}};
    CVar<Color> TitleBgCollapsed{"title_bg_collapsed_style", Color{}};
    CVar<Color> MenuBarBg{"menu_bar_bg_style", Color{}};
    CVar<Color> ScrollbarBg{"scrollbar_bg_style", Color{}};
    CVar<Color> ScrollbarGrab{"scrollbar_grab_style", Color{}};
    CVar<Color> ScrollbarGrabHovered{"scrollbar_grab_hovered_style", Color{}};
    CVar<Color> ScrollbarGrabActive{"scrollbar_grab_active_style", Color{}};
    CVar<Color> CheckMark{"check_mark_style", Color{}};
    CVar<Color> SliderGrab{"slider_grab_style", Color{}};
    CVar<Color> SliderGrabActive{"slider_grab_active_style", Color{}};
    CVar<Color> Button{"button_style", Color{}};
    CVar<Color> ButtonHovered{"button_hovered_style", Color{}};
    CVar<Color> ButtonActive{"button_active_style", Color{}};
    CVar<Color> Header{"header_style", Color{}};
    CVar<Color> HeaderHovered{"header_hovered_style", Color{}};
    CVar<Color> HeaderActive{"header_active_style", Color{}};
    CVar<Color> Separator{"separator_style", Color{}};
    CVar<Color> SeparatorHovered{"separator_hovered_style", Color{}};
    CVar<Color> SeparatorActive{"separator_active_style", Color{}};
    CVar<Color> ResizeGrip{"resize_grip_style", Color{}};
    CVar<Color> ResizeGripHovered{"resize_grip_hovered_style", Color{}};
    CVar<Color> ResizeGripActive{"resize_grip_active_style", Color{}};
    CVar<Color> PlotLines{"plot_lines_style", Color{}};
    CVar<Color> PlotLinesHovered{"plot_lines_hovered_style", Color{}};
    CVar<Color> PlotHistogram{"plot_histogram_style", Color{}};
    CVar<Color> PlotHistogramHovered{"plot_histogram_hovered_style", Color{}};
    CVar<Color> TextSelectedBg{"text_selected_bg_style", Color{}};
    CVar<Color> DragDropTarget{"drag_drop_target_style", Color{}};
    CVar<Color> NavHighlight{"nav_highlight_style", Color{}};
    CVar<Color> NavWindowingHighlight{"nav_windowing_highlight_style", Color{}};
    CVar<Color> NavWindowingDimBg{"nav_windowing_dim_bg_style", Color{}};
    CVar<Color> ModalWindowDimBg{"modal_window_dim_bg_style", Color{}};
  };

 private:
  GUIModule() = default;
  ~GUIModule() = default;

  void StartUp(const GLFWwindow* win);
  void Update(float deltaTime);
  void ShutDown();

  // TODO(Jacob)
  // ImGui::SetAllocatorFunctions line 1383

  const GLFWwindow* winHandle;
  int winWidth, winHeight;

  friend class EngineLoop;
  friend class StackAllocator;
};
}  // namespace Isetta
