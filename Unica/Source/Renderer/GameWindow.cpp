// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "GameWindow.h"

#include "UnicaInstance.h"

GameWindow::GameWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_GlfwWindow = glfwCreateWindow(1280, 900, "Unica Engine", nullptr, nullptr);
}

void GameWindow::Tick()
{
    if (glfwWindowShouldClose(m_GlfwWindow))
    {
        UnicaInstance::RequestExit();
    }
    glfwPollEvents();
}

GameWindow::~GameWindow()
{
    glfwDestroyWindow(m_GlfwWindow);
    glfwTerminate();
}
