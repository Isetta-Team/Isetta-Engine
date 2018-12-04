/*
 * Copyright (c) 2018 Isetta
 */
#include "Window.h"

#include "Core/Math/Rect.h"
#include "Core/Math/Vector2Int.h"
#include "Texture.h"
#include "WindowModule.h"
#include "imgui/imgui.h"

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
  GLFWimage images[1];
  images[0].pixels = static_cast<unsigned char*>(icon.GetData());
  images[0].width = icon.GetWidth();
  images[0].height = icon.GetHeight();
  glfwSetWindowIcon(windowModule->winHandle, 1, images);
}

void Window::SetTitle(const std::string_view title) {
  glfwSetWindowTitle(windowModule->winHandle, title.data());
}

void Window::SetSizeLimits(const Math::Rect& size) {
  glfwSetWindowSizeLimits(windowModule->winHandle, size.x, size.y, size.width,
                          size.height);
}

void Window::SetFullscreen(bool fullscreen) {
  if (IsFullscreen() == fullscreen) return;
  windowModule->SetFullscreen(fullscreen);
}

bool Window::IsFullscreen() {
  return glfwGetWindowMonitor(windowModule->winHandle);
}

void Window::SetCursorIcon(const Cursor icon) {
  windowModule->cursor = icon;
  ImGui::SetMouseCursor(static_cast<ImGuiMouseCursor>(icon));
}

void Window::SetCursorIcon(const Texture& icon,
                           const Math::Vector2Int& hotspot) {
  GLFWimage image;
  image.pixels = static_cast<unsigned char*>(icon.GetData());
  image.width = icon.GetWidth();
  image.height = icon.GetHeight();
  SetCursorIcon(Cursor::Custom);
  if (windowModule->customCursor) glfwDestroyCursor(windowModule->customCursor);
  windowModule->customCursor = glfwCreateCursor(&image, hotspot.x, hotspot.y);
  glfwSetCursor(windowModule->winHandle, windowModule->customCursor);
  glfwSetInputMode(windowModule->winHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::SetCursorMode(const CursorMode& mode) {
  ImGuiIO& io = ImGui::GetIO();
  switch (mode) {
    case CursorMode::Normal: {
      glfwSetInputMode(windowModule->winHandle, GLFW_CURSOR,
                       GLFW_CURSOR_NORMAL);
      io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
      ImGui::SetMouseCursor(
          static_cast<ImGuiMouseCursor>(windowModule->cursor));
      break;
    }
    case CursorMode::Hidden: {
      glfwSetInputMode(windowModule->winHandle, GLFW_CURSOR,
                       GLFW_CURSOR_HIDDEN);
      io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
      windowModule->cursor = static_cast<Cursor>(ImGui::GetMouseCursor());
      ImGui::SetMouseCursor(ImGuiMouseCursor_None);
      break;
    }
    case CursorMode::Disabled: {
      io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
      glfwSetInputMode(windowModule->winHandle, GLFW_CURSOR,
                       GLFW_CURSOR_DISABLED);
      break;
    }
  }
}
}  // namespace Isetta
