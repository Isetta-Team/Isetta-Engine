/*
 * Copyright (c) 2018 Isetta
 */
#include "Window.h"

#include "Core/Math/Rect.h"
#include "Texture.h"
#include "WindowModule.h"

namespace Isetta {
WindowModule* Window::windowModule = nullptr;

int Window::GetWidth() {
  int width;
  glfwGetWindowSize(windowModule->winHandle, &width, nullptr);
  return width;
}

int Window::GetHeight() {
  int height;
  glfwGetWindowSize(windowModule->winHandle, nullptr, &height);
  return height;
}

void Window::SetIcon(const Texture& icon) {
  GLFWimage image;
  unsigned int tex = icon.GetTexture();
  // TODO(Jacob) Test, may not work as expected
  image.pixels = reinterpret_cast<unsigned char*>(&tex);
  image.width = icon.GetWidth();
  image.height = icon.GetHeight();
  glfwSetWindowIcon(windowModule->winHandle, 1, &image);
}

void Window::SetTitle(const std::string_view title) {
  glfwSetWindowTitle(windowModule->winHandle, title.data());
}

void Window::SetSizeLimits(const Math::Rect& size) {
  glfwSetWindowSizeLimits(windowModule->winHandle, size.x, size.y, size.width,
                          size.height);
}

void Window::SetFullScreen() {
  GLFWmonitor* monitor = glfwGetWindowMonitor(windowModule->winHandle);
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwSetWindowMonitor(windowModule->winHandle, monitor, 0, 0, mode->width,
                       mode->height, mode->refreshRate);
}
}  // namespace Isetta
