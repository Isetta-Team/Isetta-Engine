/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "GLFW/include/GLFW/glfw3.h"

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
    CVar<Color> text{"text_style", Color{}};
    CVar<Color> textDisabled{"text_disabled_style", Color{}};
    CVar<Color> windowBg{"window_bg_style_style", Color{}};
    CVar<Color> childBg{"child_bg_style", Color{}};
    CVar<Color> popupBg{"popup_bg_style", Color{}};
    CVar<Color> border{"border_style", Color{}};
    CVar<Color> borderShadow{"border_shadow_style", Color{}};
    CVar<Color> frameBg{"frame_bg_style", Color{}};
    CVar<Color> frameBgHovered{"frame_bg_hovered_style", Color{}};
    CVar<Color> frameBgActive{"frame_bg_active_style", Color{}};
    CVar<Color> titleBg{"title_bg_style", Color{}};
    CVar<Color> titleBgActive{"title_bg_active_style", Color{}};
    CVar<Color> titleBgCollapsed{"title_bg_collapsed_style", Color{}};
    CVar<Color> menuBarBg{"menu_bar_bg_style", Color{}};
    CVar<Color> scrollbarBg{"scrollbar_bg_style", Color{}};
    CVar<Color> scrollbarGrab{"scrollbar_grab_style", Color{}};
    CVar<Color> scrollbarGrabHovered{"scrollbar_grab_hovered_style", Color{}};
    CVar<Color> scrollbarGrabActive{"scrollbar_grab_active_style", Color{}};
    CVar<Color> checkMark{"check_mark_style", Color{}};
    CVar<Color> sliderGrab{"slider_grab_style", Color{}};
    CVar<Color> sliderGrabActive{"slider_grab_active_style", Color{}};
    CVar<Color> button{"button_style", Color{}};
    CVar<Color> buttonHovered{"button_hovered_style", Color{}};
    CVar<Color> buttonActive{"button_active_style", Color{}};
    CVar<Color> header{"header_style", Color{}};
    CVar<Color> headerHovered{"header_hovered_style", Color{}};
    CVar<Color> headerActive{"header_active_style", Color{}};
    CVar<Color> separator{"separator_style", Color{}};
    CVar<Color> separatorHovered{"separator_hovered_style", Color{}};
    CVar<Color> separatorActive{"separator_active_style", Color{}};
    CVar<Color> resizeGrip{"resize_grip_style", Color{}};
    CVar<Color> resizeGripHovered{"resize_grip_hovered_style", Color{}};
    CVar<Color> resizeGripActive{"resize_grip_active_style", Color{}};
    CVar<Color> plotLines{"plot_lines_style", Color{}};
    CVar<Color> plotLinesHovered{"plot_lines_hovered_style", Color{}};
    CVar<Color> plotHistogram{"plot_histogram_style", Color{}};
    CVar<Color> plotHistogramHovered{"plot_histogram_hovered_style", Color{}};
    CVar<Color> textSelectedBg{"text_selected_bg_style", Color{}};
    CVar<Color> dragDropTarget{"drag_drop_target_style", Color{}};
    CVar<Color> navHighlight{"nav_highlight_style", Color{}};
    CVar<Color> navWindowingHighlight{"nav_windowing_highlight_style", Color{}};
    CVar<Color> navWindowingDimBg{"nav_windowing_dim_bg_style", Color{}};
    CVar<Color> modalWindowDimBg{"modal_window_dim_bg_style", Color{}};
    CVar<float> defaultFontSize{"default_font_size", 16.f};
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
