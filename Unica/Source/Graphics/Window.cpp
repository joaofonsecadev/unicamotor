// Copyright João Fonseca, All Rights Reserved.

#include "UnicaPch.h"
#include "Window.h"

Window::Window(const string WindowTitle, const int32 WindowWidth, const int32 WindowHeight)
{
    m_WindowTitle = WindowTitle;
    m_WindowWidth = WindowWidth;
    m_WindowHeight = WindowHeight;
}

void Window::Init()
{
    if (!glfwInit())
    {
        UNICA_LOG(Error, "Could not initialize GLFW!");
    }

    // Disable OpenGL context creation and resizable window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowTitle.c_str(), m_Monitor, nullptr);
}

void Window::Tick()
{
    glfwPollEvents();
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
