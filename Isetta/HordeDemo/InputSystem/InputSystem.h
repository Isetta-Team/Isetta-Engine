#pragma once
#include "GLFW/glfw3.h"

class InputSystem {
public:
    InputSystem();
    ~InputSystem();

    void RegisterWindowCloseCallback(GLFWwindow* win, GLFWwindowclosefun cb);
    void RegisterWindowSizeCallback(GLFWwindow* win, GLFWwindowsizefun cb);
    void RegisterKeyCallback(GLFWwindow* win, GLFWkeyfun cb);
};

