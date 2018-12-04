/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/WindowModule.h"

#include "Core/Config/Config.h"
#include "Window.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {
void Isetta::WindowModule::StartUp() {
  glfwInit();
  InitWindow();
  Window::windowModule = this;
}
void WindowModule::Update(float deltaTime) {
  BROFILER_CATEGORY("Window Update", Profiler::Color::Silver);

  if (glfwWindowShouldClose(winHandle)) {
    Application::Exit();
  }
  glfwSwapBuffers(winHandle);
}

void WindowModule::ShutDown() {
  glfwDestroyWindow(winHandle);
  winHandle = nullptr;
  glfwTerminate();
}

void WindowModule::InitWindow() {  // Create OpenGL window
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (CONFIG_VAL(windowConfig.windowFullScreen)) {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    winHandle = glfwCreateWindow(mode->width, mode->height,
                                 CONFIG_VAL(windowConfig.windowTitle).c_str(),
                                 glfwGetPrimaryMonitor(), nullptr);
  } else {
    winHandle = glfwCreateWindow(CONFIG_VAL(windowConfig.windowWidth),
                                 CONFIG_VAL(windowConfig.windowHeight),
                                 CONFIG_VAL(windowConfig.windowTitle).c_str(),
                                 nullptr, nullptr);
  }

  if (winHandle == nullptr) {
    // Fake message box
    glfwDestroyWindow(winHandle);

    winHandle = glfwCreateWindow(800, 50,
                                 "Unable to initialize engine - Make sure you "
                                 "have an OpenGL 2.0 compatible graphics card",
                                 nullptr, nullptr);
    double startTime = glfwGetTime();
    while (glfwGetTime() - startTime < 5.0) { /* Sleep */
    }

    throw std::exception(
        "Render::InitWindow: Unable to initialize window. Make sure you have "
        "an OpenGL compatible graphics card.");
  }

  glfwSetWindowUserPointer(winHandle, this);
  glfwMakeContextCurrent(winHandle);
  // TODO(Chaojie): Probally remove this line in the future and limit the max
  // fps to 60fps
  glfwSwapInterval(0);

  glfwSetInputMode(winHandle, GLFW_CURSOR,
                   CONFIG_VAL(windowConfig.windowShowCursor)
                       ? GLFW_CURSOR_NORMAL
                       : GLFW_CURSOR_DISABLED);
}

void WindowModule::SetFullscreen(bool fullscreen) {
  if (fullscreen) {
    glfwGetWindowPos(winHandle, &xPos, &yPos);
    glfwGetWindowSize(winHandle, &width, &height);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(winHandle, monitor, 0, 0, mode->width, mode->height,
                         mode->refreshRate);
  } else {
    glfwSetWindowMonitor(winHandle, nullptr, xPos, yPos, width, height, 0);
  }
}

// GLFWmonitor* WindowModule::GetCurrentMonitor() const {
//  GLFWmonitor* current = NULL;
//  GLFWmonitor** monitors;
//  GLFWvidmode* mode;
//  int monitorCnt;
//  monitors = glfwGetMonitors(&monitorCnt);
//
//  int posX, posY, width, height;
//  glfwGetWindowPos(winHandle, &posX, &posY);
//  glfwGetWindowSize(winHandle, &width, &height);
//
//  int x, y, w, h;
//  for (int i = 0; i < monitorCnt; ++i) {
//    const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
//    glfwGetMonitorPos(monitors[i], &x, &y);
//
//    w = mode->width;
//    h = mode->height;
//
//    if (currentOverlap < overlap) {
//      currentOverlap = overlap;
//      current = monitors[i];
//    }
//  }
//  return current;
//}
}  // namespace Isetta
