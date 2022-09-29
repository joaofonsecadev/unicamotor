// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "RenderWindow.h"

#include "UnicaInstance.h"
#include "UnicaMinimal.h"

RenderWindow::RenderWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_GlfwWindow = glfwCreateWindow(1280, 900, "Unica Engine", nullptr, nullptr);

    UNICA_LOG(Log, "LogRenderWindow", "Created application window");
}

void RenderWindow::Tick()
{
    if (glfwWindowShouldClose(m_GlfwWindow))
    {
        UnicaInstance::RequestExit();
    }
    glfwPollEvents();
}

RenderWindow::~RenderWindow()
{
    glfwDestroyWindow(m_GlfwWindow);
    glfwTerminate();

    UNICA_LOG(Log, "LogRenderWindow", "Destroyed application window");
}
