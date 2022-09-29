// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class RenderWindow
{
public:
    RenderWindow();
    ~RenderWindow();

    void Tick();

private:
    GLFWwindow* m_GlfwWindow;
};
