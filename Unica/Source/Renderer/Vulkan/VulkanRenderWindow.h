// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GlfwRenderWindow
{
public:
    GlfwRenderWindow();
    ~GlfwRenderWindow();

    void Tick();

    GLFWwindow* GetGlfwWindow() const { return m_GlfwWindow; }

private:
    void WindowCloseRequested();
    void PollEvents();
    
    GLFWwindow* m_GlfwWindow;
};