#include "InputSystem.h"


InputSystem::InputSystem() {
}


InputSystem::~InputSystem() {
}

void InputSystem::RegisterWindowCloseCallback(GLFWwindow* win, GLFWwindowclosefun cb) {
    glfwSetWindowCloseCallback(win, cb);
}

void InputSystem::RegisterWindowSizeCallback(GLFWwindow * win, GLFWwindowsizefun cb) {
    glfwSetWindowSizeCallback(win, cb);
}

void InputSystem::RegisterKeyCallback(GLFWwindow * win, GLFWkeyfun cb) {
    glfwSetKeyCallback(win, cb);
}
